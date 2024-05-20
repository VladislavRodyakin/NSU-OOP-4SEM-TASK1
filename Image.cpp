#include "Image.hpp"
#include <exception>
#include <cstring>

unsigned char *Image::take_line(int elem_count, int pos, int direction) const
{
    unsigned char* line = new unsigned char[elem_count*m_channels];
    if (direction == 1){
        for (int i = 0; i < elem_count; i++){
            //extracting coloumn
            if (elem_count == m_rows){ 
                memcpy(line + i*m_channels, m_data + (pos + i*m_cols)*m_channels, m_channels);
            }
            //extracting row
            if (elem_count == m_cols){
                memcpy(line + i*m_channels, m_data + (pos*m_cols + i)*m_channels, m_channels);
            }
        }
    }
    //for rotate90 and vertical mirror we need to take line backwards
    else if (direction == -1){
        for (int i = elem_count - 1; i >= 0; i--){
            //memcpy(line + i*m_channels, m_data + i*pos*m_channels*elem_count, m_channels);
            //extracting coloumn
            if (elem_count == m_rows){ 
                memcpy(line + (elem_count - i - 1)*m_channels, m_data + (pos + i*m_cols)*m_channels, m_channels);
            }
            //extracting row
            if (elem_count == m_cols){
                memcpy(line + (elem_count - i - 1)*m_channels, m_data + (pos*m_cols + i)*m_channels, m_channels);
            }
        }
    }
    else{
        throw std::invalid_argument("Invalid direction argument");
    }
    return line;
}

void Image::rotate90(int direction) // 1 clockwise, -1 counterclockwise
{
    int tmp_rows = m_cols;
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    unsigned char* tmp_col_backwards;
    for (int i = 0; i < m_cols; i++){
        tmp_col_backwards = take_line(m_rows, i, -direction);
        memcpy(tmp_data + i*m_rows*m_channels, tmp_col_backwards, m_rows*m_channels);
    }
    m_cols = m_rows;
    m_rows = tmp_rows;
    m_data = tmp_data;
}

Image::Image() = default;

Image::Image(int rows, int cols, int channels):
    m_rows{rows>0?rows:throw std::invalid_argument("Invalid rows")},
    m_cols{cols>0?cols:throw std::invalid_argument("Invalid cols")},
    m_channels{channels>0?channels:throw std::invalid_argument("Invalid channels")}
{}

Image::Image(int rows, int cols, int channels, unsigned char *data): Image(rows, cols, channels)
{
    m_count_refs = new size_t{1};
    m_data=data;
}

Image::Image(const Image &image): m_rows{image.m_rows}, m_cols{image.m_cols},
                             m_channels{image.m_channels}, m_data{image.m_data},
                             m_count_refs{image.m_count_refs}
{
    if (m_count_refs != nullptr){
        (*m_count_refs)++;
    }
}

Image::~Image()
{
    this->release();
}

Image& Image::operator=(const Image &image)
{
    if (this == &image){
        return *this;
    }
    this->release();
    m_rows = image.m_rows;
    m_cols = image.m_cols;
    m_channels = image.m_channels;
    m_data = image.m_data;
    m_count_refs = image.m_count_refs;
    if (m_count_refs != nullptr){
        (*m_count_refs)++;
    }
    return *this;
}

Image Image::clone() const
{
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    if (m_data != nullptr) {
        memcpy(tmp_data, m_data, m_cols*m_rows*m_channels);
    }
    return Image(m_rows, m_cols, m_channels, tmp_data);
}

void Image::copyTo(Image &image) const
{
    image=this->clone(); // is it? (it is)
}

void Image::create(int rows, int cols, int channels)
{
    if (rows<=0 || cols<=0 || channels<=0){
        throw std::invalid_argument("Invalid arguments");
    }
    this->release();
    m_rows = rows;
    m_cols = cols;
    m_channels = channels;
    m_data = new unsigned char[m_cols*m_rows*m_channels];
    m_count_refs = new size_t{1};
    std::memset(m_data, 0, m_cols*m_rows*m_channels);
}

bool Image::empty() const
{
    return m_data==nullptr;
}

void Image::release()
{
    if (m_count_refs == nullptr){
        return; //this is better to just return bc polymorphysm
    }
    
    (*m_count_refs)--;
    if ((*m_count_refs)<=0){
        delete m_count_refs;
        delete m_data; //simple obj -> no delete[]
        m_count_refs = nullptr;
        m_data = nullptr;
    }
}

Image Image::col(int x) const
{
    if (x>=m_cols){
        throw std::out_of_range("Index out of range.");
    }
    unsigned char* line = take_line(m_rows, x);
    Image result(m_rows, 1, m_channels, line);
    return result;
}

Image Image::row(int y) const
{
    if (y>=m_rows){
        throw std::out_of_range("Index out of range.");
    }
    unsigned char* line = take_line(m_cols, y);
    Image result(1, m_cols, m_channels, line);
    return result;
}

const unsigned char* Image::data() const
{
    return m_data;
}

unsigned char *Image::data()
{
    return m_data;
}

int Image::rows() const
{
    return m_rows;
}

int Image::cols() const
{
    return m_cols;
}

int Image::total() const
{
    return m_cols*m_rows;
}

int Image::channels() const
{
    return m_channels;
}

unsigned char& Image::at(int index)
{
    if (index>=m_channels*m_cols*m_rows||index<0){
        throw std::out_of_range("Index out of range.");
    }
    return m_data[index];
}

const unsigned char& Image::at(int index) const
{
    if (index>=m_channels*m_cols*m_rows||index<0){
        throw std::out_of_range("Index out of range.");
    }
    return m_data[index];
}

Image Image::zeros(int rows, int cols, int channels)
{
    Image result(rows, cols, channels);
    memset((result.m_data), 0, rows*cols*channels);
    return result;
}

Image Image::values(int rows, int cols, int channels, unsigned char value)
{
    Image result(rows, cols, channels);
    memset(&(result.m_data), value, rows*cols*channels);
    return result;
}

void Image::Mirror(MirrorType type)
{
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    unsigned char* tmp_row;
    switch (type)
    {
    case MirrorType::Vertical:
        //take pieces sizeof m_cols*m_channels for i<m_rows and place at [len - m_rows*m_channels*i]
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i);
            memcpy(tmp_data + (m_rows - i - 1)*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
        break;
    case MirrorType::Horizontal:
        //take pieces sizeof m_channels at (i*m_channels)*(j*m_cols) for i<m_cols j<m_rows
        //and place them at (m_cols*m_channels)*(j*m_cols) - m_cols*m_channels*i
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i, -1);
            memcpy(tmp_data + i*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
        break;
    default:
        break;
    }
    m_count_refs = new size_t{1};
    m_data = tmp_data;
}

void Image::Rotate(double angle)
{
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    std::memcpy(tmp_data, m_data, m_cols*m_rows*m_channels);
    this->release();
    m_data = tmp_data;
    int i_angle = static_cast<int>(angle);

    if (i_angle%90 != 0) 
    {
            throw std::invalid_argument("Invalid turn angle");
    }

    
    //(angle/90) % 4 = number of 90grad rotations
    int num_rot = (i_angle/90)%4;
    switch (num_rot)
    {
    case 1:
        this->rotate90();
        break;
    case 2:
        /*
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i, -1);
            memcpy(tmp_data + (m_rows - i)*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
        *this = Image(m_rows, m_cols, m_channels, tmp_data);
        */
        // redo combining Mirroring algorithms
        this->rotate90();
        this->rotate90();
        break;
    case 3:
        this->rotate90(-1);
        break;
    default:
        break;
    }
    
    m_count_refs = new size_t{1};
    
}

size_t Image::countRef() const
{
    if (m_count_refs == nullptr){
        return 0;
    }
    return (*m_count_refs);
}
