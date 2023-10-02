#include "brightnesshandler.h"
#include <string>


BrightnessHandler::BrightnessHandler(QObject *parent)
    : QObject{parent}
{
    brightnessFile.open(BRIGHTNESS_PATH);
}

BrightnessHandler::~BrightnessHandler()
{
    brightnessFile.close();
}

int BrightnessHandler::getBrightness()
{
    int realBrightness;
    std::string line;
    std::getline(brightnessFile, line);
    realBrightness = std::stoi(line);
    brightnessFile.seekg(0);
    return static_cast<int>(realBrightness / MAX_BRIGHTNESS * 100);
}

void BrightnessHandler::setBrightness(int brightness)
{
    int convBrightness = (brightness / 100.0) * MAX_BRIGHTNESS;
    brightnessFile << std::to_string(convBrightness) << std::endl;
    emit brightnessChanged();
}
