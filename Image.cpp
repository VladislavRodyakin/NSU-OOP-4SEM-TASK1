#include "Image.hpp"

Image::Image()
{}

Image::Image(int rows, int cols, int channels): m_rows{rows}, m_cols{cols}, m_channels{channels}
{}

Image::Image(int rows, int cols, int channels, unsigned char *data): Image(rows, cols, channels)
{
    m_data=data;
}

Image::Image(const Image &image): m_rows{image.m_rows}, m_cols{image.m_cols}, m_channels{image.m_channels}, m_data{image.m_data}
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
    m_rows = image.m_rows;
    m_cols = image.m_cols;
    m_channels = image.m_channels;
    m_data = image.m_data;
    *m_count_refs++;
    return *this;
}

Image Image::clone()
{
    return Image(*this);
}

void Image::copyTo(Image &image)
{
    image=this->clone(); // is it?
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
    m_count_refs--;
    if (*m_count_refs<=0){
        delete m_data;
    }
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
    if (type == MirrorType::Vertical)
    {
        
    }
    else if (type == MirrorType::Horizontal)
    {
        
    }
}

void Image::Rotate(double angle)
{
    int i_angle = static_cast<int>(angle);
    if (i_angle%90 != 0) 
    {
        //exception
        // return?
    }
    //(angle/360 * 4) % 4 = number of 90grad rotations
}
