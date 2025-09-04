#include "DrawingTool.h"

using namespace std;

DrawingTool::DrawingTool()
    : m_toolType(ToolType::Curve)
    , m_penColor(Qt::black)
    , m_penWidth(2)
{
}

void DrawingTool::setToolType(ToolType type)
{
    m_toolType = type;
}

ToolType DrawingTool::getToolType() const
{
    return m_toolType;
}

void DrawingTool::setPenColor(const QColor& color)
{
    m_penColor = color;
}

void DrawingTool::setPenWidth(int width)
{
    m_penWidth = qMax(1, width);
}

QColor DrawingTool::getPenColor() const
{
    return m_penColor;
}

int DrawingTool::getPenWidth() const
{
    return m_penWidth;
}

QPen DrawingTool::getPen() const
{
    QPen pen(m_penColor);
    pen.setWidth(m_penWidth);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    return pen;
}

void DrawingTool::reset()
{
    m_toolType = ToolType::Curve;
    m_penColor = Qt::black;
    m_penWidth = 2;
}
