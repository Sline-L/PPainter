#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <QImage>
#include <QString>

class ImageUtils
{
public:
    static QImage load_image(const QString &file_path);
    static bool save_image(const QImage &image, const QString &file_path);
    static QImage resize_image(const QImage &image, const QSize &size);
    static QImage convert_to_pixel_art(const QImage &image, int pixel_size);
    
    // 【扩展点】添加更多图像处理功能
    // 【扩展点】添加滤镜功能
    // 【扩展点】添加格式转换功能
};

#endif // IMAGE_UTILS_H
