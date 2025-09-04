#ifndef SELECTION_TOOL_H
#define SELECTION_TOOL_H

#include "drawing_tool.h"
#include <QRect>

class SelectionTool : public DrawingTool
{
    Q_OBJECT

public:
    explicit SelectionTool(QObject *parent = nullptr);
    
    void mouse_press(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_move(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_release(const QPoint &pos, PixelCanvas *canvas) override;
    
    QRect selection_rect() const { return selection_rect_; }

private:
    QPoint start_pos_;
    QRect selection_rect_;
    
    // 【扩展点】添加选择区域操作功能
};

#endif // SELECTION_TOOL_H
