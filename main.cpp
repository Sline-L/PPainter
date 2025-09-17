#include <QApplication>
#include "src/MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("PPainter");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("PPainter Whiteboard");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
