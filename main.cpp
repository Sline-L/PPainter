#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>

#include "src/ui/main_window.h"
#include "config/app_config.h"
#include "src/utils/image_utils.h"

using namespace std;

// 应用程序信息
const QString APP_NAME = "PixelWhiteboard";
const QString APP_VERSION = "1.0.0";
const QString APP_ORGANIZATION = "PixelStudio";

// 初始化应用程序配置
void init_application_config(QApplication& app)
{
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName(APP_ORGANIZATION);
    app.setApplicationDisplayName("Pixel Whiteboard - 多人协作像素画板");
}

// 设置应用程序样式
void setup_application_style(QApplication& app)
{
    // 设置应用程序图标
    app.setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    // 设置现代化样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 应用自定义样式表
    QFile style_file(":/styles/app_style.qss");
    if (style_file.open(QFile::ReadOnly)) {
        QString style_sheet = QLatin1String(style_file.readAll());
        app.setStyleSheet(style_sheet);
        style_file.close();
    }
}

// 初始化国际化
void setup_internationalization(QApplication& app)
{
    QTranslator* translator = new QTranslator(&app);
    
    // 获取系统语言
    QString locale = QLocale::system().name();
    
    // 加载对应语言文件
    QString translation_file = QString(":/translations/pixelwhiteboard_%1.qm").arg(locale);
    if (translator->load(translation_file)) {
        app.installTranslator(translator);
    }
}

// 检查应用程序依赖
bool check_dependencies()
{
    // 检查必要的目录是否存在
    QString config_dir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QString app_config_dir = config_dir + "/" + APP_ORGANIZATION + "/" + APP_NAME;
    
    QDir dir;
    if (!dir.exists(app_config_dir)) {
        if (!dir.mkpath(app_config_dir)) {
            QMessageBox::critical(nullptr, "错误", 
                "无法创建配置目录: " + app_config_dir);
            return false;
        }
    }
    
    // 检查临时目录
    QString temp_dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (!QDir(temp_dir).exists()) {
        QMessageBox::critical(nullptr, "错误", 
            "临时目录不可用: " + temp_dir);
        return false;
    }
    
    return true;
}

// 显示启动画面
QSplashScreen* show_splash_screen()
{
    QPixmap splash_pixmap(":/images/splash_screen.png");
    if (splash_pixmap.isNull()) {
        // 如果没有启动画面图片，创建一个简单的
        splash_pixmap = QPixmap(400, 300);
        splash_pixmap.fill(Qt::white);
        
        QPainter painter(&splash_pixmap);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(splash_pixmap.rect(), Qt::AlignCenter, 
            APP_NAME + "\n版本 " + APP_VERSION + "\n\n正在启动...");
    }
    
    QSplashScreen* splash = new QSplashScreen(splash_pixmap);
    splash->show();
    splash->showMessage("正在初始化应用程序...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
    
    return splash;
}

// 应用程序清理
void cleanup_application()
{
    // 保存应用程序配置
    AppConfig::instance().save_config();
    
    // 清理临时文件
    ImageUtils::cleanup_temp_files();
}

int main(int argc, char *argv[])
{
    // 启用高DPI支持
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    // 创建应用程序对象
    QApplication app(argc, argv);
    
    // 初始化应用程序配置
    init_application_config(app);
    
    // 检查依赖
    if (!check_dependencies()) {
        return -1;
    }
    
    // 显示启动画面
    QSplashScreen* splash = show_splash_screen();
    
    // 处理启动事件
    app.processEvents();
    
    try {
        // 设置样式
        splash->showMessage("正在加载样式...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
        app.processEvents();
        setup_application_style(app);
        
        // 初始化国际化
        splash->showMessage("正在加载语言包...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
        app.processEvents();
        setup_internationalization(app);
        
        // 初始化应用程序配置
        splash->showMessage("正在加载配置...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
        app.processEvents();
        AppConfig::instance().load_config();
        
        // 创建主窗口
        splash->showMessage("正在创建主窗口...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);
        app.processEvents();
        
        MainWindow main_window;
        
        // 延迟显示主窗口，让启动画面显示更久一点
        QTimer::singleShot(1000, [&]() {
            splash->finish(&main_window);
            main_window.show();
            delete splash;
        });
        
        // 设置清理函数
        QObject::connect(&app, &QApplication::aboutToQuit, cleanup_application);
        
        // 运行应用程序
        return app.exec();
        
    } catch (const exception& e) {
        // 处理C++异常
        splash->hide();
        delete splash;
        
        QMessageBox::critical(nullptr, "严重错误", 
            QString("应用程序发生异常: %1").arg(e.what()));
        return -1;
        
    } catch (...) {
        // 处理未知异常
        splash->hide();
        delete splash;
        
        QMessageBox::critical(nullptr, "严重错误", 
            "应用程序发生未知异常");
        return -1;
    }
}
