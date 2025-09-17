#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include <QColor>
#include <QPen>

enum class ToolType {
    Line,
    Rectangle,
    Curve,
    Text    // 新增：文本工具
};

class DrawingTool
{
public:
    DrawingTool();
    
    // 工具类型
    void setToolType(ToolType type);
    ToolType getToolType() const;
    
    // 画笔设置
    void setPenColor(const QColor& color);
    void setPenWidth(int width);
    
    QColor getPenColor() const;
    int getPenWidth() const;
    QPen getPen() const;
    
    // 重置为默认设置
    void reset();

private:
    ToolType m_tool_type;
    QColor m_pen_color;
    int m_pen_width;
};

#endif // DRAWINGTOOL_H
