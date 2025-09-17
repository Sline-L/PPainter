#include "Imagepaster.h"
#include "DrawingArea.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMessageBox>
#include <qrubberband.h>

ImagePaster::ImagePaster(DrawingArea *area)
    : QObject(area),
    m_area(area),
    m_enabled(false),
    m_dragging(false)
{
    // 安装事件过滤器到画布上
    m_area->installEventFilter(this);
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, m_area);
    m_rubberBand->hide();
}

void ImagePaster::chooseImage()
{
    // 弹出文件对话框，只限 png, jpg
    QString fileName = QFileDialog::getOpenFileName(
        m_area,
        tr("选择图片"),
        QString(),
        tr("图片文件 (*.png *.jpg)"));
    if (fileName.isEmpty())
        return;

    // 载入到 QPixmap
    if (!m_pixmap.load(fileName)) {
        QMessageBox::warning(m_area,
                             tr("加载失败"),
                             tr("无法加载图片：%1").arg(fileName));
        return;
    }

    // 切换到贴图模式
    m_enabled = true;
}

void ImagePaster::cancel()
{
    // 退出贴图模式
    m_enabled = false;
    m_dragging = false;
    m_area->update();  // 清除可能残留的虚线框
}

bool ImagePaster::eventFilter(QObject *watched, QEvent *event)
{
    if (!m_enabled || watched != m_area)
        return QObject::eventFilter(watched, event);

    switch(event->type()) {
    case QEvent::MouseButtonPress: {
        auto *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton) {
            m_startPoint = me->pos();
            m_endPoint   = m_startPoint;
            m_dragging   = true;

            // 显示一个 0 大小的 rubber band
            m_rubberBand->setGeometry(QRect(m_startPoint, QSize()));
            m_rubberBand->show();

            return true;
        }
        break;
    }
    case QEvent::MouseMove: {
        if (m_dragging) {
            auto *me = static_cast<QMouseEvent*>(event);
            m_endPoint = me->pos();

            // 更新 rubber band 的大小/位置
            QRect r(m_startPoint, m_endPoint);
            m_rubberBand->setGeometry(r.normalized());
            return true;
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        auto *me = static_cast<QMouseEvent*>(event);
        if (m_dragging && me->button() == Qt::LeftButton) {
            m_dragging = false;
            m_endPoint = me->pos();

            // 拖完就隐藏 rubber band
            m_rubberBand->hide();

            // 下面这段照搬你原来把图贴到 canvas 的逻辑
            QRect target = QRect(m_startPoint, m_endPoint).normalized();
            if (!target.isEmpty()) {
                QPixmap scaled = m_pixmap.scaled(
                    target.size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation);
                QSize  s  = scaled.size();
                QPoint tl = target.topLeft();
                QRect  dst(
                    tl.x() + (target.width()  - s.width())/2,
                    tl.y() + (target.height() - s.height())/2,
                    s.width(),
                    s.height());
                m_area->pasteImage(scaled, dst);
            }
            m_enabled = false;
            return true;
        }
        break;
    }
    default:
        break;
    }

    return QObject::eventFilter(watched, event);
}
