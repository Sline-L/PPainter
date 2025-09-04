#include "app_config.h"
#include <QSettings>
#include <QStandardPaths>

using namespace std;

AppConfig& AppConfig::instance()
{
    static AppConfig instance;
    return instance;
}

void AppConfig::load_config(const QString &file_path)
{
    QString config_path = file_path.isEmpty() ?
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.ini" :
        file_path;

    QSettings settings(config_path, QSettings::IniFormat);

    default_canvas_size_ = settings.value("canvas/size", QSize(800, 600)).toSize();
    default_port_ = settings.value("network/port", 8888).toUInt();
    // 注意：这里使用 QColor(Qt::white) 作为默认值（QVariant 支持 QColor）
    default_background_color_ = settings.value("ui/background_color", QColor(Qt::white)).value<QColor>();

    // 【扩展点】加载更多配置项
}

void AppConfig::save_config(const QString &file_path) const
{
    QString config_path = file_path.isEmpty() ?
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.ini" :
        file_path;

    QSettings settings(config_path, QSettings::IniFormat);

    settings.setValue("canvas/size", default_canvas_size_);
    settings.setValue("network/port", default_port_);
    settings.setValue("ui/background_color", default_background_color_);

    // 【扩展点】保存更多配置项
}
