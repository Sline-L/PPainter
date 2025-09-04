#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include <QWidget>
#include <QColor>

class ColorPalette : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPalette(QWidget *parent = nullptr);
    
    QColor current_color() const { return current_color_; }

signals:
    void color_changed(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QColor current_color_;
    // 【扩展点】添加调色板颜色数组
    // 【扩展点】添加自定义颜色功能
};

#endif // COLOR_PALETTE_H
