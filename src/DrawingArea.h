#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QLineEdit>
#include "DrawingTool.h"

class MainWindow;

class DrawingArea : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit DrawingArea(QWidget *parent = nullptr, MainWindow *window = nullptr);

    // 工具设置
    void setDrawingTool(const DrawingTool& tool);
    DrawingTool getDrawingTool() const;
    MainWindow *window;

    // 画布操作
    void clearCanvas();
    void saveImage(const QString& fileName);
    void loadImage(const QString& fileName);
    //新增
    void pasteImage(const QPixmap &pixmap, const QRect &targetRect);

    // 画布大小
    void resizeCanvas(const QSize& size);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLine(const QPoint &startPoint, const QPoint &endPoint, const QColor color, const int width);
    void drawRectangle(const QPoint &startPoint, const QPoint &endPoint, const QPoint &firstcornerPoint, const QPoint &secondcornerPoint);
    void initializeCanvas();

    // 将 text 绘制到画布
    void commitTextEdit();

private:
    QPixmap m_canvas;           // 画布
    DrawingTool m_drawing_tool;  // 当前绘图工具

    bool m_drawing;             // 是否正在绘制
    QPoint m_last_point;        // 上一个点
    QPoint m_start_point;       // 起始点（用于直线工具）
    QPoint m_firstcorner_point;  // 角落的点一（用于矩形工具）
    QPoint m_secondcorner_point; // 角落的点二（用于矩形工具）

    QVector<QPoint> m_current_path;  // 当前绘制路径

    // 文本编辑相关
    QLineEdit *m_text_edit;     // 在画布上临时输入文本
    QPoint m_text_edit_pos;
};

#endif // DRAWINGAREA_H
