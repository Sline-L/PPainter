#include "DrawingArea.h"
#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QDebug>
#include <QFont>
#include <QEvent>

using namespace std;

DrawingArea::DrawingArea(QWidget *parent, MainWindow *window)
    : QWidget(parent)
    , m_drawing(false)
    , window(window)
    , m_text_edit(nullptr)
{
    setAttribute(Qt::WA_StaticContents);
    setMinimumSize(800, 600);
    initializeCanvas();
}

void DrawingArea::initializeCanvas()
{
    QSize canvas_size = size();
    if (canvas_size.isEmpty()) {
        canvas_size = QSize(800, 600);
    }

    m_canvas = QPixmap(canvas_size);
    m_canvas.fill(Qt::white);
    update();
}

void DrawingArea::setDrawingTool(const DrawingTool& tool)
{
    m_drawing_tool = tool;
}

DrawingTool DrawingArea::getDrawingTool() const
{
    return m_drawing_tool;
}

void DrawingArea::clearCanvas()
{
    // 清除可能存在的文本编辑器
    if (m_text_edit) {
        m_text_edit->deleteLater();
        m_text_edit = nullptr;
    }

    m_canvas.fill(Qt::white);
    update();
}

void DrawingArea::saveImage(const QString& fileName)
{
    if (!m_canvas.save(fileName)) {
        QMessageBox::warning(this, tr("保存失败"),
                             tr("无法保存图片到 %1").arg(fileName));
    }
}

void DrawingArea::loadImage(const QString& fileName)
{
    QPixmap loaded_pixmap(fileName);
    if (loaded_pixmap.isNull()) {
        QMessageBox::warning(this, tr("加载失败"),
                             tr("无法加载图片 %1").arg(fileName));
        return;
    }

    m_canvas = loaded_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void DrawingArea::resizeCanvas(const QSize& size)
{
    if (size == m_canvas.size()) {
        return;
    }

    QPixmap new_canvas(size);
    new_canvas.fill(Qt::white);

    QPainter painter(&new_canvas);
    painter.drawPixmap(0, 0, m_canvas);

    m_canvas = new_canvas;
    update();
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, m_canvas, dirtyRect);

    // 如果正在绘制直线，显示预览线
    if (m_drawing && m_drawing_tool.getToolType() == ToolType::Line) {
        painter.setPen(m_drawing_tool.getPen());
        painter.drawLine(m_start_point, m_last_point);
    }
    else if (m_drawing && m_drawing_tool.getToolType() == ToolType::Rectangle) {
        painter.setPen(m_drawing_tool.getPen());
        painter.drawLine(m_start_point, m_firstcorner_point);
        painter.drawLine(m_start_point, m_secondcorner_point);
        painter.drawLine(m_firstcorner_point, m_last_point);
        painter.drawLine(m_secondcorner_point, m_last_point);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 若当前是文本工具，弹出输入框，不进入绘制
        if (m_drawing_tool.getToolType() == ToolType::Text) {
            // 若已有编辑框，则先提交
            if (m_text_edit) {
                commitTextEdit();
            }

            m_text_edit_pos = event->pos();
            m_text_edit = new QLineEdit(this);
            m_text_edit->setFrame(false);
            m_text_edit->setAttribute(Qt::WA_DeleteOnClose);
            m_text_edit->move(m_text_edit_pos);
            m_text_edit->setFixedWidth(200);
            m_text_edit->show();
            m_text_edit->setFocus();

            // 回车或失去焦点时提交文本到画布
            connect(m_text_edit, &QLineEdit::editingFinished, this, [this]() {
                commitTextEdit();
            });

            return;
        }

        m_drawing = true;
        m_last_point = event->pos();
        m_start_point = event->pos();
        m_firstcorner_point = event->pos();
        m_secondcorner_point = event->pos();
        m_current_path.clear();
        m_current_path.append(m_last_point);
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_drawing) {
        QPoint current_point = event->pos();

        if (m_drawing_tool.getToolType() == ToolType::Curve) {
            // 曲线绘制：实时绘制到画布
            window->sendMessage(m_last_point, current_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            m_last_point = current_point;
        }
        else if (m_drawing_tool.getToolType() == ToolType::Rectangle) {
            // 矩形绘制：只更新预览
            m_last_point = current_point;
            m_firstcorner_point = QPoint(m_start_point.x(), m_last_point.y());
            m_secondcorner_point = QPoint(m_last_point.x(), m_start_point.y());
            update();
        }
        else {
            // 直线绘制：只更新预览
            m_last_point = current_point;
            update();  // 触发重绘显示预览线
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        QPoint end_point = event->pos();
        QPoint finalfirstcorner_point = QPoint(m_start_point.x(), end_point.y());
        QPoint finalsecondcorner_point = QPoint(end_point.x(), m_start_point.y());

        if (m_drawing_tool.getToolType() == ToolType::Line) {
            // 直线绘制：在鼠标释放时绘制最终直线
            window->sendMessage(m_start_point, end_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            update();
        }
        else if (m_drawing_tool.getToolType() == ToolType::Rectangle) {
            window->sendMessage(m_start_point, finalfirstcorner_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            window->sendMessage(m_start_point, finalsecondcorner_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            window->sendMessage(finalfirstcorner_point, end_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            window->sendMessage(finalsecondcorner_point, end_point, m_drawing_tool.getPenColor(), m_drawing_tool.getPenWidth());
            update();
        }

        m_drawing = false;
        m_current_path.clear();
    }
}

void DrawingArea::resizeEvent(QResizeEvent *event)
{
    resizeCanvas(event->size());
    QWidget::resizeEvent(event);
}

void DrawingArea::drawLine(const QPoint &startPoint, const QPoint &endPoint, const QColor color, const int width)
{
    QPainter painter(&m_canvas);
    QPen pen(color);
    pen.setWidth(width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(startPoint, endPoint);

    int rad = m_drawing_tool.getPenWidth() / 2 + 2;
    QRect updateRect = QRect(startPoint, endPoint).normalized()
                           .adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);
}

void DrawingArea::drawRectangle(const QPoint &startPoint, const QPoint &endPoint, const QPoint &firstcornerPoint, const QPoint &secondcornerPoint)
{
    QPainter painter(&m_canvas);
    painter.setPen(m_drawing_tool.getPen());
    painter.drawLine(startPoint, firstcornerPoint);
    painter.drawLine(startPoint, secondcornerPoint);
    painter.drawLine(firstcornerPoint, endPoint);
    painter.drawLine(secondcornerPoint, endPoint);

    // 更新需要重绘的区域
    int rad = m_drawing_tool.getPenWidth() / 2 + 2;
    QRect updateRect = QRect(startPoint, endPoint).normalized()
                           .adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);
}

void DrawingArea::pasteImage(const QPixmap &pixmap, const QRect &targetRect)
{
    if (pixmap.isNull()) return;
    QPainter p(&m_canvas);
    p.drawPixmap(targetRect.topLeft(), pixmap);
    p.end();
    update(targetRect);
}

void DrawingArea::commitTextEdit()
{
    if (!m_text_edit) return;

    QString text = m_text_edit->text();
    if (!text.isEmpty()) {
        QPainter painter(&m_canvas);
        QPen pen = m_drawing_tool.getPen();
        painter.setPen(pen);

        // 使用适度的字体（你可根据需要调整）
        QFont font = painter.font();
        font.setPointSize(14);
        painter.setFont(font);

        painter.drawText(m_text_edit_pos.x(), m_text_edit_pos.y() + font.pointSize(), text);
        update(QRect(m_text_edit_pos, QSize(300, 50)));
    }

    m_text_edit->deleteLater();
    m_text_edit = nullptr;
}
