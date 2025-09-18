#include "Imagepaster.h"
#include "DrawingArea.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMessageBox>
#include <qrubberband.h>
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QVariant>
#include <QImage>

using namespace std;

ImagePaster::ImagePaster(DrawingArea *area)
    : QObject(area),
    m_area(area),
    m_enabled(false),
    m_dragging(false)
{
    m_area->installEventFilter(this);
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, m_area);
    m_rubberBand->hide();
}

void ImagePaster::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        m_area,
        tr("选择图片"),
        QString(),
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if (fileName.isEmpty())
        return;

    if (!m_pixmap.load(fileName)) {
        QMessageBox::warning(m_area,
                             tr("加载失败"),
                             tr("无法加载图片：%1").arg(fileName));
        return;
    }

    m_enabled = true;
}

void ImagePaster::pasteFromClipboard()
{
    const QClipboard *clipboard = QGuiApplication::clipboard();
    if (!clipboard) return;

    QImage img = clipboard->image();
    if (!img.isNull()) {
        m_pixmap = QPixmap::fromImage(img);
        m_enabled = true;
        return;
    }

    const QMimeData *mime = clipboard->mimeData();
    if (mime && mime->hasImage()) {
        QVariant v = mime->imageData();
        QImage im = qvariant_cast<QImage>(v);
        if (!im.isNull()) {
            m_pixmap = QPixmap::fromImage(im);
            m_enabled = true;
            return;
        }
    }

    QMessageBox::information(m_area, tr("剪贴板无图像"), tr("剪贴板中没有可用的图像"));
}

void ImagePaster::cancel()
{
    m_enabled = false;
    m_dragging = false;
    m_area->update();
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

            m_rubberBand->hide();

            QRect target = QRect(m_startPoint, m_endPoint).normalized();
            if (!target.isEmpty() && !m_pixmap.isNull()) {
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
