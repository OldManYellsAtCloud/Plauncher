#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>

#include "settings.h"

#include "launchermodel.h"
#include "visibilityhandler.h"
#include "brightnesshandler.h"
#include "shutdown.h"

int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Material");
    QGuiApplication app(argc, argv);
    app.setOrganizationName("My Pine Stuff");
    app.setOrganizationDomain("sgy.pine");

    Settings::getSettings().init("/etc/launcher");

    QQmlApplicationEngine engine;

    qmlRegisterType<LauncherModel>("sgy.pine.launcher", 1, 0, "LauncherModel");
    qmlRegisterType<VisibilityHandler>("sgy.pine.launcher", 1, 0, "VisiblityHandler");
    qmlRegisterType<BrightnessHandler>("sgy.pine.launcher", 1, 0, "BrightnessHandler");
    qmlRegisterType<Shutdown>("sgy.pine.launcher", 1, 0, "ShutdownHandler");

    const QUrl url(u"qrc:/Launcher/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
