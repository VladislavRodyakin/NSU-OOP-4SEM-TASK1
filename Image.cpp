#include "Image.hpp"
#include <exception>
#include <cstring>

unsigned char *Image::take_line(int elem_count, int pos, int direction) const
{
    std::cout<<"takeline pre succesfull, dir: "<<direction<<" elem: "<<elem_count<<std::endl;

    unsigned char* line = new unsigned char[elem_count*m_channels];
    if (direction == 1){
        for (int i = 0; i < elem_count; i++){
        //     for (int j = 0; j < m_channels; j++){
        //         line[i*m_channels+j] = m_data[i*pos*m_channels+j];
        //     }
            //std::cout<<"takeline pre - memcpy succesfull"<<std::endl;
            memcpy(line + i*m_channels, m_data + i*pos*m_channels, m_channels);
            std::cout<<"takeline memcpy succesfull"<<std::endl;
        }
    }

    //for rotate90 and vertical mirror we need to take line backwards
    if (direction == -1){
        for (int i = elem_count; i > 0; i--){
            //std::cout<<"takeline pre - memcpy succesfull"<<std::endl;
            memcpy(line + i*m_channels, m_data + i*pos*m_channels, m_channels);
            std::cout<<"takeline memcpy succesfull"<<std::endl;
        }
    }
    else{
        throw std::invalid_argument("Invalid direction argument");
    }
    return line;
}

void Image::rotate90(int direction) // 1 clockwise, -1 counterclockwise
{
    //int tmp_rows = m_cols;
    //int tmp_cols = m_rows;
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    unsigned char* tmp_col_backwards;
    for (int i = 0; i < m_cols; i++){
        tmp_col_backwards = take_line(m_rows, i, -direction);
        memcpy(tmp_data + i*m_rows*m_channels, tmp_col_backwards, m_rows*m_channels);
        std::cout<<"rotate cycle succesfull"<<std::endl;
    }
    std::cout<<(*tmp_data)<<std::endl;
    //*this = Image(m_cols, m_rows, m_channels, tmp_data);
    m_cols = m_rows;
    m_rows = m_cols;
    delete m_data;
    m_data = tmp_data;
}

Image::Image()
{}

Image::Image(int rows, int cols, int channels): m_rows{rows}, m_cols{cols}, m_channels{channels}
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
    this->create(image.m_rows, image.m_cols, image.m_channels);
    /*
    this->release();
    m_rows = image.m_rows;
    m_cols = image.m_cols;
    m_channels = image.m_channels;
    */
    m_data = image.m_data;
    m_count_refs = image.m_count_refs;
    if (m_count_refs != nullptr){
        (*m_count_refs)++;
    }
    return *this;
}

Image Image::clone()
{
    return Image(*this);
}

void Image::copyTo(Image &image)
{
    image=this->clone(); // is it? (it is)
}

void Image::create(int rows, int cols, int channels)
{
    this->release();
    m_rows = rows;
    m_cols = cols;
    m_channels = channels;
}

bool Image::empty() const
{
    return m_data==nullptr;
}

void Image::release()
{
    if (m_count_refs == nullptr){
        //throw std::exception("Releasing empty image");
        //this is better to just return bc polymorphysm
        return;
    }
    (*m_count_refs)--;
    if ((*m_count_refs)<=0){
        delete m_count_refs;
        //delete[] m_data; //simple obj -> no delete[]
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
    if (index>=m_channels*m_cols*m_rows){
        throw std::out_of_range("Index out of range.");
    }
    return m_data[index];
}

const unsigned char& Image::at(int index) const
{
    if (index>=m_channels*m_cols*m_rows){
        throw std::out_of_range("Index out of range.");
    }
    return m_data[index];
}

Image Image::zeros(int rows, int cols, int channels)
{
    Image result(rows, cols, channels);
    memset(&(result.m_data), 0, rows*cols*channels);
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
            memcpy(tmp_data + (m_rows - i)*m_cols*m_channels, tmp_row, m_cols*m_channels);
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
    *this = Image(m_rows, m_cols, m_channels, tmp_data);
}

void Image::Rotate(double angle)
{
    int i_angle = static_cast<int>(angle);
    //std::cout<<"angle got1"<<std::endl;

    if (i_angle%90 != 0) 
    {
            throw std::invalid_argument("Invalid turn angle");
    }

    //(angle/90) % 4 = number of 90grad rotations
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    unsigned char* tmp_row;
    int num_rot = (i_angle/90)%4;
    std::cout<<"angle got: "<< num_rot<<std::endl;
    switch (num_rot)
    {
    case 1:
        std::cout<<"case 1"<<std::endl;
        this->rotate90();
        break;
    case 2:
        std::cout<<"case 2"<<std::endl;
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i, -1);
            memcpy(tmp_data + (m_rows - i)*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
        *this = Image(m_rows, m_cols, m_channels, tmp_data);
        break;
    case 3:
        std::cout<<"case 3"<<std::endl;
        this->rotate90(-1);
        break;
    default:
        break;
    }
}

size_t Image::countRef()
{
    if (m_count_refs == nullptr){
        return 0;
        //we can throw but why? nullptr means we're not referencing anything even once
    }
    return *m_count_refs;
}
