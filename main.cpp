#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>

#include "settings.h"
#include "launchermodel.h"

#include "visibilityhandler.h"
int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Material");
    QGuiApplication app(argc, argv);

    Settings::getSettings().init("/etc/launcher");

    QQmlApplicationEngine engine;

    qmlRegisterType<LauncherModel>("sgy.pine.bus", 1, 0, "LauncherModel");
    qmlRegisterType<VisibilityHandler>("sgy.pine.bus", 1, 0, "VisiblityHandler");

    const QUrl url(u"qrc:/Launcher/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
