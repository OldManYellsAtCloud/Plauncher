#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>

#include "settings.h"

#include "launchermodel.h"
#include "visibilityhandler.h"
#include "brightnesshandler.h"
#include "taskhandler.h"

#include "sway_utils.h"

#define ORG_NAME  "MyPine"
#define ORG_DOMAIN  "sgy.pine"

#define SETTINGS_PATH  "/etc/launcher"


int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Material");
    QGuiApplication app(argc, argv);
    // needed to persist settings by Qt
    app.setOrganizationName(ORG_NAME);
    app.setOrganizationDomain(ORG_DOMAIN);

    Settings::getSettings().init(SETTINGS_PATH);

    QQmlApplicationEngine engine;

    qmlRegisterType<LauncherModel>("sgy.pine.launcher", 1, 0, "LauncherModel");
    qmlRegisterType<VisibilityHandler>("sgy.pine.launcher", 1, 0, "VisiblityHandler");
    qmlRegisterType<BrightnessHandler>("sgy.pine.launcher", 1, 0, "BrightnessHandler");
    qmlRegisterType<TaskHandler>("sgy.pine.launcher", 1, 0, "TaskHandler");

    const QUrl url(u"qrc:/Launcher/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    /*send_sway_message("", message_type::GET_TREE);
    send_sway_message("[\"window\"]", message_type::SUBSCRIBE, 0);*/

    return app.exec();
}
