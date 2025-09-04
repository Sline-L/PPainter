#include <QApplication>
#include "src/ui/main_window.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
