#include <iostream>

enum class MirrorType {
  Vertical,
  Horizontal
};

class Image {
    size_t* m_count_refs; 
    int m_rows, m_cols, m_channels;
    unsigned char* m_data;
    unsigned char* take_line(int elem_count, int pos, int direction = 1) const;
    void rotate90(int direction = 1);
public:
    Image();
    Image(int rows, int cols, int channels);
    Image(int rows, int cols, int channels, unsigned char* data);
    Image(const Image& image);
    virtual ~Image();

    Image& operator=(const Image& image);

    // Вернуть клон изборажения
    Image clone();
    //Скопировать изображение.
    void copyTo(Image& image); // efficently clone, but into existing obj
    void create(int rows, int cols, int channels);
    bool empty() const;

    //декрементирует счетчик ссылок и в случае необходимости освобождает ресурсы (память).
    void release();

    Image col(int x) const;

    Image row(int y) const;

    const unsigned char* data() const;
    unsigned char* data();

    int rows() const;
    int cols() const;
    int total() const;
    int channels() const;

    //Вернуть ЧАСТЬ пикселя
    unsigned char& at(int index);
    const unsigned char& at(int index) const;

    // not obj-bound funcs --- static
    static Image zeros(int rows, int cols, int channels);
    static Image values(int rows, int cols, int channels, unsigned char value);

    //Отразить изображение по вертикали или по горизонтали
    void Mirror(MirrorType type);

    //Повернуть на угол кратный 90
    void Rotate(double angle); //not div 90 => exception

    //Возвращает текущее количество ссылок на изображение.
    //Т.е. количество объектов, которые ссылаются на данное изображение. Этот метод нужен для unit test'ов.
    size_t countRef();
};