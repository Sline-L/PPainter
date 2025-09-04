#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include "DrawingTool.h"

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);
    
    // 工具设置
    void setDrawingTool(const DrawingTool& tool);
    DrawingTool getDrawingTool() const;
    
    // 画布操作
    void clearCanvas();
    void saveImage(const QString& fileName);
    void loadImage(const QString& fileName);
    
    // 画布大小
    void resizeCanvas(const QSize& size);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void drawLine(const QPoint &startPoint, const QPoint &endPoint);
    void drawRectangle(const QPoint &startPoint, const QPoint &endPoint, const QPoint &firstcornerPoint, const QPoint &secondcornerPoint);
    void initializeCanvas();

private:
    QPixmap m_canvas;           // 画布
    DrawingTool m_drawingTool;  // 当前绘图工具
    
    bool m_drawing;             // 是否正在绘制
    QPoint m_lastPoint;         // 上一个点
    QPoint m_startPoint;        // 起始点（用于直线工具）
    QPoint m_firstcornerPoint;  // 角落的点一（用于矩形工具）
    QPoint m_secondcornerPoint; // 角落的点二（用于矩形工具）
    
    QVector<QPoint> m_currentPath;  // 当前绘制路径
};

#endif // DRAWINGAREA_H
