#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include <QColor>
#include <QPen>

enum class ToolType {
    Line,
    Rectangle,
    Curve,
    Text,
    Eraser
};

class DrawingTool
{
public:
    DrawingTool();
    
    void setToolType(ToolType type);
    ToolType getToolType() const;
    
    void setPenColor(const QColor& color);
    void setPenWidth(int width);
    
    QColor getPenColor() const;
    int getPenWidth() const;
    QPen getPen() const;
    
    void reset();

private:
    ToolType m_tool_type;
    QColor m_pen_color;
    int m_pen_width;
};

#endif // DRAWINGTOOL_H
