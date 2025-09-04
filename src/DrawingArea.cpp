#include "DrawingArea.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>

using namespace std;

DrawingArea::DrawingArea(QWidget *parent)
    : QWidget(parent)
    , m_drawing(false)
{
    setAttribute(Qt::WA_StaticContents);
    setMinimumSize(800, 600);
    initializeCanvas();
}

void DrawingArea::initializeCanvas()
{
    QSize canvasSize = size();
    if (canvasSize.isEmpty()) {
        canvasSize = QSize(800, 600);
    }
    
    m_canvas = QPixmap(canvasSize);
    m_canvas.fill(Qt::white);
    update();
}

void DrawingArea::setDrawingTool(const DrawingTool& tool)
{
    m_drawingTool = tool;
}

DrawingTool DrawingArea::getDrawingTool() const
{
    return m_drawingTool;
}

void DrawingArea::clearCanvas()
{
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
    QPixmap loadedPixmap(fileName);
    if (loadedPixmap.isNull()) {
        QMessageBox::warning(this, tr("加载失败"), 
                           tr("无法加载图片 %1").arg(fileName));
        return;
    }
    
    m_canvas = loadedPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void DrawingArea::resizeCanvas(const QSize& size)
{
    if (size == m_canvas.size()) {
        return;
    }
    
    QPixmap newCanvas(size);
    newCanvas.fill(Qt::white);
    
    QPainter painter(&newCanvas);
    painter.drawPixmap(0, 0, m_canvas);
    
    m_canvas = newCanvas;
    update();
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, m_canvas, dirtyRect);
    
    // 如果正在绘制直线，显示预览线
    if (m_drawing && m_drawingTool.getToolType() == ToolType::Line) {
        painter.setPen(m_drawingTool.getPen());
        painter.drawLine(m_startPoint, m_lastPoint);
    }
    else if (m_drawing && m_drawingTool.getToolType() == ToolType::Rectangle) {
        painter.setPen(m_drawingTool.getPen());
        painter.drawLine(m_startPoint, m_firstcornerPoint);
        painter.drawLine(m_startPoint, m_secondcornerPoint);
        painter.drawLine(m_firstcornerPoint, m_lastPoint);
        painter.drawLine(m_secondcornerPoint, m_lastPoint);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drawing = true;
        m_lastPoint = event->pos();
        m_startPoint = event->pos();
        m_firstcornerPoint = event->pos();
        m_secondcornerPoint = event->pos();
        m_currentPath.clear();
        m_currentPath.append(m_lastPoint);
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_drawing) {
        QPoint currentPoint = event->pos();
        
        if (m_drawingTool.getToolType() == ToolType::Curve) {
            // 曲线绘制：实时绘制到画布
            drawLineTo(currentPoint);
        } 
        else if (m_drawingTool.getToolType() == ToolType::Rectangle) {
            // 矩阵绘制：只更新预览
            m_lastPoint = currentPoint;
            m_firstcornerPoint = QPoint(m_startPoint.x(), m_lastPoint.y());
            m_secondcornerPoint = QPoint(m_lastPoint.x(), m_startPoint.y());
            update();
        }
        else {
            // 直线绘制：只更新预览
            m_lastPoint = currentPoint;
            update();  // 触发重绘显示预览线
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        QPoint endPoint = event->pos();
        QPoint finalfirstcornerPoint = QPoint(m_startPoint.x(), endPoint.y());
        QPoint finalsecondcornerPoint = QPoint(endPoint.x(), m_startPoint.y());
        
        if (m_drawingTool.getToolType() == ToolType::Line) {
            // 直线绘制：在鼠标释放时绘制最终直线
            drawLine(m_startPoint, endPoint);
        }
        else if (m_drawingTool.getToolType() == ToolType::Rectangle) {
            drawLine(m_startPoint, finalfirstcornerPoint);
            drawLine(m_startPoint, finalsecondcornerPoint);
            drawLine(finalfirstcornerPoint, endPoint);
            drawLine(finalsecondcornerPoint, endPoint);
        }
        

        m_drawing = false;
        m_currentPath.clear();
    }
}

void DrawingArea::resizeEvent(QResizeEvent *event)
{
    resizeCanvas(event->size());
    QWidget::resizeEvent(event);
}

void DrawingArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&m_canvas);
    painter.setPen(m_drawingTool.getPen());
    painter.drawLine(m_lastPoint, endPoint);
    
    // 更新需要重绘的区域
    int rad = m_drawingTool.getPenWidth() / 2 + 2;
    QRect updateRect = QRect(m_lastPoint, endPoint).normalized()
                      .adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);
    
    m_lastPoint = endPoint;
}

void DrawingArea::drawLine(const QPoint &startPoint, const QPoint &endPoint)
{
    QPainter painter(&m_canvas);
    painter.setPen(m_drawingTool.getPen());
    painter.drawLine(startPoint, endPoint);
    
    // 更新需要重绘的区域
    int rad = m_drawingTool.getPenWidth() / 2 + 2;
    QRect updateRect = QRect(startPoint, endPoint).normalized()
                      .adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);
}

void DrawingArea::drawRectangle(const QPoint &startPoint, const QPoint &endPoint, const QPoint &firstcornerPoint, const QPoint &secondcornerPoint)
{
    QPainter painter(&m_canvas);
    painter.setPen(m_drawingTool.getPen());
    painter.drawLine(startPoint, firstcornerPoint);
    painter.drawLine(startPoint, secondcornerPoint);
    painter.drawLine(firstcornerPoint, endPoint);
    painter.drawLine(secondcornerPoint, endPoint);
    
    // 更新需要重绘的区域
    int rad = m_drawingTool.getPenWidth() / 2 + 2;
    QRect updateRect = QRect(startPoint, endPoint).normalized()
                      .adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);
}