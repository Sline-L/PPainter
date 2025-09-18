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
#include <QRubberBand>
#include "DrawingTool.h"

class MainWindow;

class DrawingArea : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit DrawingArea(QWidget *parent = nullptr, MainWindow *window = nullptr);

    void setDrawingTool(const DrawingTool& tool);
    DrawingTool getDrawingTool() const;
    MainWindow *window;

    void clearCanvas();
    void saveImage(const QString& fileName);
    void loadImage(const QString& fileName);
    void pasteImage(const QPixmap &pixmap, const QRect &targetRect);

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

    void commitTextEdit();

private:
    QPixmap m_canvas;
    DrawingTool m_drawing_tool;

    bool m_drawing;
    QPoint m_last_point;
    QPoint m_start_point;
    QPoint m_firstcorner_point;
    QPoint m_secondcorner_point;

    QVector<QPoint> m_current_path;

    QLineEdit *m_text_edit;
    QPoint m_text_edit_pos;

    // 橡皮擦用的 rubber band
    QRubberBand *m_eraser_rubberBand;
    bool m_eraser_dragging;
    QPoint m_eraser_start;
    QPoint m_eraser_end;
};

#endif // DRAWINGAREA_H
