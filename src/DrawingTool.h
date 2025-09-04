#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include <QColor>
#include <QPen>

enum class ToolType {
    Line,
    Rectangle,
    Curve
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
    ToolType m_toolType;
    QColor m_penColor;
    int m_penWidth;
};

#endif // DRAWINGTOOL_H
