#include <mplayer/mpvobject.hpp>
#include <mplayer/mpvrenderer.hpp>

#include <stdexcept>
#include <clocale>

#include <sync/host.hpp>
#include <sync/client.hpp>
#include <sync/clientinfo.hpp>
#include <sync/clientinfomodel.hpp>

// Qt
#include <QObject>
#include <QtGlobal>
#include <QOpenGLContext>
#include <QGuiApplication>
#include <QMetaObject>
#include <QQmlContext>

#include <QtGui/QOpenGLFramebufferObject>

// Qt Quick
#include <QtQuick/QQuickWindow>
#include <QQmlApplicationEngine>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // Qt sets the locale in the QGuiApplication constructor, but libmpv
    // requires the LC_NUMERIC category to be set to "C", so change it back.
    std::setlocale(LC_NUMERIC, "C");

    qmlRegisterType<mplayer::MpvObject>("syncmpv", 1, 0, "MpvObject");
    qmlRegisterType<sync::Host>("syncmpv", 1, 0, "Host");
    qmlRegisterType<sync::Client>("syncmpv", 1, 0, "Client");

    QQmlApplicationEngine engine;
    sync::ClientInfoModel model;

    engine.rootContext()->setContextProperty("clientInfoModel", &model);
    engine.load("view/main.qml");

    mplayer::MpvObject* mpv = engine.rootObjects().first()->findChild<mplayer::MpvObject*>("mpv");
    sync::Host* host = engine.rootObjects().first()->findChild<sync::Host*>("host");
    sync::Client* client = engine.rootObjects().first()->findChild<sync::Client*>("client");

    client->setMpv(mpv);
    host->setClientInfoModel(&model);
    host->setMpv(mpv);

    QObject::connect(mpv, &mplayer::MpvObject::stateChanged, host, &sync::Host::broadcastPlayerState);

    return app.exec();

}
