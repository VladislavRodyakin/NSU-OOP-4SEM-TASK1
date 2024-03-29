#include "Image.hpp"
#include <exception>

unsigned char *Image::take_line(int elem_coount, int pos, int direction)
{
    unsigned char* line = new unsigned char[elem_coount*m_channels];
    if (direction == 1){
        for (int i = 0; i < elem_coount; i++){
        //     for (int j = 0; j < m_channels; j++){
        //         line[i*m_channels+j] = m_data[i*pos*m_channels+j];
        //     }
            memcpy(line + i*m_channels, m_data + i*pos*m_channels, m_channels);
        }
    }

    //for rotate90 and vertical mirror we need to take line backwards
    if (direction == -1){
        for (int i = elem_coount; i >=0; i--){
            memcpy(line + i*m_channels, m_data + i*pos*m_channels, m_channels);
        }
    }
    else{
        throw std::exception("Invalid direction argument");
    }
    return line;
}

void Image::rotate90()
{
    //int tmp_rows = m_cols;
    //int tmp_cols = m_rows;
    unsigned char* tmp_data = new unsigned char[m_cols*m_rows*m_channels];
    unsigned char* tmp_col_backwards;
    for (int i = 0; i < m_cols; i++){
        tmp_col_backwards = take_line(m_rows, i, -1);
        memcpy(tmp_data + i*m_rows*m_channels, tmp_col_backwards, m_rows*m_channels);
    }
    *this = Image(m_cols, m_rows, m_channels, tmp_data);
}

Image::Image()
{}

Image::Image(int rows, int cols, int channels): m_rows{rows}, m_cols{cols}, m_channels{channels}
{}

Image::Image(int rows, int cols, int channels, unsigned char *data): Image(rows, cols, channels)
{
    m_data=data;
}

Image::Image(const Image &image): m_rows{image.m_rows}, m_cols{image.m_cols},
                             m_channels{image.m_channels}, m_data{image.m_data}
{
    *m_count_refs++;
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
    *m_count_refs++;
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

bool Image::empty()
{
    return m_data==nullptr;
}

void Image::release()
{
    *m_count_refs--;
    if (*m_count_refs<=0){
        delete m_data;
    }
}

Image Image::col(int x)
{
    unsigned char* line = take_line(m_rows, x);
    Image result(m_rows, 1, m_channels, line);
    return result;
}

Image Image::row(int y)
{
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

int Image::rows()
{
    return m_rows;
}

int Image::cols()
{
    return m_cols;
}

int Image::total()
{
    return m_cols*m_rows;
}

int Image::channels()
{
    return m_channels;
}

unsigned char& Image::at(int index)
{
    return m_data[index];
}

const unsigned char& Image::at(int index) const
{
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
    if (type == MirrorType::Vertical)
    {
        //take pieces sizeof m_cols*m_channels for i<m_rows and place at [len - m_rows*m_channels*i]
        unsigned char* tmp_row;
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i);
            memcpy(tmp_data + (m_rows - i)*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
    }
    else if (type == MirrorType::Horizontal)
    {
        //take pieces sizeof m_channels at (i*m_channels)*(j*m_cols) for i<m_cols j<m_rows
        //and place them at (m_cols*m_channels)*(j*m_cols) - m_cols*m_channels*i
        unsigned char* tmp_row;
        for(int i = 0; i < m_rows; i++){
            tmp_row = take_line(m_cols, i, -1);
            memcpy(tmp_data + i*m_cols*m_channels, tmp_row, m_cols*m_channels);
        }
    }
    *this = Image(m_rows, m_cols, m_channels, tmp_data);
}

void Image::Rotate(double angle)
{
    int i_angle = static_cast<int>(angle);
    if (i_angle%90 != 0) 
    {
            throw std::exception("Invalid turn angle");
    }

    //(angle/90) % 4 = number of 90grad rotations
    for (int i = 0; i < ((i_angle / 90) % 4); i++){
        //rotation cycle, rotating clockwise
        this->rotate90();
    }
}

size_t Image::countRef()
{
    return *m_count_refs;
}
