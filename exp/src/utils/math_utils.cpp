#include "math_utils.h"
#include <cmath>
#include <algorithm>

using namespace std;

double MathUtils::distance(const QPoint &p1, const QPoint &p2)
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    return sqrt(dx * dx + dy * dy);
}

QVector<QPoint> MathUtils::line_points(const QPoint &start, const QPoint &end)
{
    QVector<QPoint> points;
    
    // Bresenham直线算法
    int x0 = start.x(), y0 = start.y();
    int x1 = end.x(), y1 = end.y();
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        points.append(QPoint(x0, y0));
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    
    return points;
}

QRect MathUtils::bounding_rect(const QVector<QPoint> &points)
{
    if (points.isEmpty()) return QRect();
    
    int min_x = points[0].x(), max_x = points[0].x();
    int min_y = points[0].y(), max_y = points[0].y();
    
    for (const QPoint &point : points) {
        min_x = min(min_x, point.x());
        max_x = max(max_x, point.x());
        min_y = min(min_y, point.y());
        max_y = max(max_y, point.y());
    }
    
    return QRect(min_x, min_y, max_x - min_x, max_y - min_y);
}

QPoint MathUtils::clamp_point(const QPoint &point, const QRect &bounds)
{
    int x = max(bounds.left(), min(bounds.right(), point.x()));
    int y = max(bounds.top(), min(bounds.bottom(), point.y()));
    return QPoint(x, y);
}
