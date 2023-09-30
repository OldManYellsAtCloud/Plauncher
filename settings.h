#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

struct Launcher{
    QString name;
    QString iconPath;
    QString command;
};

class Settings
{
private:
    std::vector<Launcher> launcherSettings;
    Settings(){}
public:
    Settings(Settings const&) = delete;
    void operator=(Settings const&) = delete;
    void init(QString path);
    std::vector<Launcher> getLauncherSettings();
    ~Settings();

    static Settings& getSettings(){
        static Settings s;
        return s;
    }
};

#endif // SETTINGS_H
