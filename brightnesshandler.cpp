#include "brightnesshandler.h"
#include <string>


BrightnessHandler::BrightnessHandler(QObject *parent)
    : QObject{parent}
{
    brightnessFile.open(BRIGHTNESS_PATH);
    if ((brightnessFile.rdstate() & std::ifstream::badbit) != 0){
        qDebug() << "Could not open brightness file!";
        exit(1);
    }
}

BrightnessHandler::~BrightnessHandler()
{
    brightnessFile.close();
}

int BrightnessHandler::getBrightness()
{
    int realBrightness;
    std::string line;
    brightnessFile.seekg(0);
    std::getline(brightnessFile, line);
    realBrightness = std::stoi(line);

    return ceil(realBrightness / MAX_BRIGHTNESS * 100);
}

void BrightnessHandler::setBrightness(int brightness)
{
    int convBrightness = (brightness / 100.0) * MAX_BRIGHTNESS;
    brightnessFile << std::to_string(convBrightness) << std::endl;
    emit brightnessChanged();
}
