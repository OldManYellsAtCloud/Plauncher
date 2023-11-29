#ifndef BRIGHTNESSHANDLER_H
#define BRIGHTNESSHANDLER_H

#include <QQmlEngine>
#include <fstream>
#include <iostream>

#define BRIGHTNESS_PATH "/sys/class/backlight/backlight/brightness"
#define MAX_BRIGHTNESS_PATH "/sys/class/backlight/backlight/max_brightness"

class BrightnessHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int brightness READ getBrightness WRITE setBrightness NOTIFY brightnessChanged)
private:
    std::fstream brightnessFile;
    std::fstream maxBrightnessFile;
    int maxBrightness;

public:
    explicit BrightnessHandler(QObject *parent = nullptr);
    ~BrightnessHandler();
    int getBrightness();
    void setBrightness(int brightness);

signals:
    void brightnessChanged();
};

#endif // BRIGHTNESSHANDLER_H
