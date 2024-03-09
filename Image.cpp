#include "Image.hpp"
#include <exception>

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

Image Image::col(int x)
{
    unsigned char* column = new unsigned char[m_rows*m_channels];
    Image result(m_rows,1,m_channels,0);
    return result;
}

Image Image::row(int y)
{
    return Image();
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
        //to avoid std::swap (if it even works at all)
        //create unsigned char[m_rows*m_cols*m_channels] (via new, bc nonconst len doesnt work)
        //take pieces sizeof m_cols*m_channels for i<m_rows and place at [len - m_rows*m_channels*i]
    }
    else if (type == MirrorType::Horizontal)
    {
        //create unsigned char[m_rows*m_cols*m_channels] (via new, bc nonconst len doesnt work)
        //take pieces sizeof m_channels at (i*m_channels)*(j*m_cols) for i<m_cols j<m_rows
        //and place them at (m_cols*m_channels)*(j*m_cols) - m_cols*m_channels*i
    }
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
        //take it into separate function void rotate90(&Image orig_img);
        int tmp_rows = m_cols;
        int tmp_cols = m_rows;
        //...
    }
}

size_t Image::countRef()
{
    return *m_count_refs;
}
