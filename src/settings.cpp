#include <loglib/loglib.h>
#include "settings.h"

#include <QFileInfo>
#include <QDir>


void Settings::init(QString path){
    QFileInfo fi(path);
    if (!fi.exists() || !fi.isDir()){
        LOG_ERROR_F("{} doesn't exists or not a folder", path.toStdString());
        throw std::logic_error("Configuration path doesn't exist.");
    }

    QDir configDir {path};
    for (const QString& f: configDir.entryList()){
        if (f.endsWith(".cfg")){
            QSettings qs(path + QDir::separator() + f, QSettings::IniFormat);
            struct Launcher l;

            l.name = qs.childGroups().at(0);
            l.command = qs.value(l.name + "/command", "-").toString();
            l.iconPath = qs.value(l.name + "/iconPath", "-").toString();
            l.appId = qs.value(l.name + "/appId", "-").toString();

            if (l.command == "-" || l.iconPath == "-"){
                LOG_ERROR_F("{} config file is borked. Skipping.", f.toStdString());
                continue;
            }

            launcherSettings.push_back(l);

        }
    }
}

std::vector<Launcher> Settings::getLauncherSettings()
{
    return launcherSettings;
}

Settings::~Settings(){
}
