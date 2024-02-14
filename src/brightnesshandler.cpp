#include "brightnesshandler.h"
#include <string>
#include <ios>

BrightnessHandler::BrightnessHandler(QObject *parent)
    : QObject{parent}
{
    brightnessFile.open(BRIGHTNESS_PATH);
    maxBrightnessFile.open(MAX_BRIGHTNESS_PATH, std::ios_base::in);

    if (brightnessFile.rdstate() != std::ifstream::goodbit){
        qDebug() << "Could not open brightness file!";
        exit(1);
    }

    if (maxBrightnessFile.rdstate() != std::ifstream::goodbit){
        qDebug() << "Could not open max_brightness file!";
        exit(1);
    }

    std::string line;
    std::getline(maxBrightnessFile, line);
    maxBrightness = stoi(line);
    maxBrightnessFile.close();
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

    return ceil(realBrightness / maxBrightness * 100);
}

void BrightnessHandler::setBrightness(int brightness)
{
    int convBrightness = (brightness / 100.0) * maxBrightness;
    brightnessFile << std::to_string(convBrightness) << std::endl;
    emit brightnessChanged();
}
