#include "DrawingTool.h"

using namespace std;

DrawingTool::DrawingTool()
    : m_tool_type(ToolType::Curve)
    , m_pen_color(Qt::black)
    , m_pen_width(2)
{
}

void DrawingTool::setToolType(ToolType type)
{
    m_tool_type = type;
}

ToolType DrawingTool::getToolType() const
{
    return m_tool_type;
}

void DrawingTool::setPenColor(const QColor& color)
{
    m_pen_color = color;
}

void DrawingTool::setPenWidth(int width)
{
    m_pen_width = qMax(1, width);
}

QColor DrawingTool::getPenColor() const
{
    return m_pen_color;
}

int DrawingTool::getPenWidth() const
{
    return m_pen_width;
}

QPen DrawingTool::getPen() const
{
    QPen pen(m_pen_color);
    pen.setWidth(m_pen_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    return pen;
}

void DrawingTool::reset()
{
    m_tool_type = ToolType::Curve;
    m_pen_color = Qt::black;
    m_pen_width = 2;
}
