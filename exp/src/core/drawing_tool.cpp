#include "drawing_tool.h"

using namespace std;

DrawingTool::DrawingTool(QObject *parent)
    : QObject(parent)
    , color_(Qt::black)
    , is_drawing_(false)
{
}
