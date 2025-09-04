#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>
#include <QColor>
#include <QSize>

class AppConfig
{
public:
    static AppConfig& instance();
    
    // 画布配置
    QSize default_canvas_size() const { return default_canvas_size_; }
    void set_default_canvas_size(const QSize &size) { default_canvas_size_ = size; }
    
    // 网络配置
    quint16 default_port() const { return default_port_; }
    void set_default_port(quint16 port) { default_port_ = port; }
    
    // UI配置
    QColor default_background_color() const { return default_background_color_; }
    void set_default_background_color(const QColor &color) { default_background_color_ = color; }
    
    // 文件操作
    void load_config(const QString &file_path = "");
    void save_config(const QString &file_path = "") const;

private:
    AppConfig() = default;
    
    QSize default_canvas_size_{800, 600};
    quint16 default_port_{8888};
    QColor default_background_color_{Qt::white};
    
    // 【扩展点】添加更多配置项
    // 【扩展点】添加用户偏好设置
};

#endif // APP_CONFIG_H
