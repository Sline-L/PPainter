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
    /// 构造时传入要贴图的绘图区域
    explicit ImagePaster(DrawingArea *area);

public slots:
    /// “插入图片”按钮触发时调用：先弹对话框选图，再进入贴图模式
    void chooseImage();

    /// 若要中途取消贴图，可调用此接口
    void cancel();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    DrawingArea *m_area;   // 目标画布
    bool         m_enabled;  // 是否正在贴图模式
    bool         m_dragging; // 是否正在拖矩形
    QPoint       m_startPoint;
    QPoint       m_endPoint;
    QPixmap      m_pixmap;   // 选好的要贴的图
    QRubberBand *m_rubberBand;    // <<< 新增
};

#endif // IMAGEPASTER_H
