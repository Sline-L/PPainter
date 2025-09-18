#ifndef IMAGEPASTER_H
#define IMAGEPASTER_H

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QRubberBand>

class DrawingArea;

class ImagePaster : public QObject
{
    Q_OBJECT
public:
    explicit ImagePaster(DrawingArea *area);

public slots:
    void chooseImage();
    void pasteFromClipboard();
    void cancel();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    DrawingArea *m_area;
    bool         m_enabled;
    bool         m_dragging;
    QPoint       m_startPoint;
    QPoint       m_endPoint;
    QPixmap      m_pixmap;
    QRubberBand *m_rubberBand;
};

#endif // IMAGEPASTER_H
