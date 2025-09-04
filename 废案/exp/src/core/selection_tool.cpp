#include "selection_tool.h"

using namespace std;

SelectionTool::SelectionTool(QObject *parent)
    : DrawingTool(parent)
{
}

void SelectionTool::mouse_press(const QPoint &pos, PixelCanvas *canvas)
{
    start_pos_ = pos;
    selection_rect_ = QRect();
}

void SelectionTool::mouse_move(const QPoint &pos, PixelCanvas *canvas)
{
    selection_rect_ = QRect(start_pos_, pos).normalized();
    // 【扩展点】触发重绘以显示选择框
}

void SelectionTool::mouse_release(const QPoint &pos, PixelCanvas *canvas)
{
    selection_rect_ = QRect(start_pos_, pos).normalized();
    // 【扩展点】完成选择操作
}
