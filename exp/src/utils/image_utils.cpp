#include "image_utils.h"
#include <QImageReader>
#include <QImageWriter>

using namespace std;

QImage ImageUtils::load_image(const QString &file_path)
{
    QImageReader reader(file_path);
    return reader.read();
}

bool ImageUtils::save_image(const QImage &image, const QString &file_path)
{
    QImageWriter writer(file_path);
    return writer.write(image);
}

QImage ImageUtils::resize_image(const QImage &image, const QSize &size)
{
    return image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QImage ImageUtils::convert_to_pixel_art(const QImage &image, int pixel_size)
{
    // 【扩展点】实现像素化算法
    QSize new_size = image.size() / pixel_size;
    QImage scaled = image.scaled(new_size, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    return scaled.scaled(image.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
}
