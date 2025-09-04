#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

#include "src/ui/main_window.h"
#include "config/app_config.h"
#include "src/utils/image_utils.h"

using namespace std;

// 应用程序信息
const QString APP_NAME = "PPainter";
const QString APP_VERSION = "0.1.0";
const QString APP_ORGANIZATION = "PP";

// 初始化应用程序配置
void init_application_config(QApplication& app)
{
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName(APP_ORGANIZATION);
    app.setApplicationDisplayName("PPainter - 多人协作像素画板");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    init_application_config(app);
    
    MainWindow main_window;
    main_window.show();
    
    return app.exec();
}