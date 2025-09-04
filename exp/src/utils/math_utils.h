#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <QPoint>
#include <QRect>
#include <QVector>

class MathUtils
{
public:
    static double distance(const QPoint &p1, const QPoint &p2);
    static QVector<QPoint> line_points(const QPoint &start, const QPoint &end);
    static QRect bounding_rect(const QVector<QPoint> &points);
    static QPoint clamp_point(const QPoint &point, const QRect &bounds);
    
    // 【扩展点】添加更多数学计算功能
    // 【扩展点】添加几何变换功能
    // 【扩展点】添加插值算法
};

#endif // MATH_UTILS_H
