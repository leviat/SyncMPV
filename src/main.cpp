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

    // Some test code

    sync::ClientInfo* cInfo = new sync::ClientInfo();
    cInfo->setAddress(QHostAddress("127.0.0.1"));
    cInfo->setName("Leo");
    cInfo->setPort(8000);
    cInfo->setBufferProgress(50);
    cInfo->setPlayProgress(20);
    model.addClientInfo(cInfo);

    engine.rootContext()->setContextProperty("clientInfoModel", &model);
    engine.load("view/main.qml");


    mplayer::MpvObject* mpv = engine.rootObjects().first()->findChild<mplayer::MpvObject*>("mpv");
    network::HostSocket* host = engine.rootObjects().first()->findChild<network::HostSocket*>("host");
    //network::Client* client = engine.rootObjects().first()->findChild<network::Client*>("client");


    //TODO QObject::connect(mpv, &mplayer::MpvObject::stateChanged, host, &network::HostSocket::broadcastPlayerState);
    return app.exec();

}
