#include <media-player/mpvobject.hpp>
#include <media-player/mpvrenderer.hpp>

#include <stdexcept>
#include <clocale>

#include <network/host.hpp>
#include <network/client.hpp>

// Qt
#include <QObject>
#include <QtGlobal>
#include <QOpenGLContext>
#include <QGuiApplication>
#include <QMetaObject>

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
    qmlRegisterType<network::Host>("syncmpv", 1, 0, "Host");
    qmlRegisterType<network::Client>("syncmpv", 1, 0, "Client");

    QQmlApplicationEngine engine("view/main.qml");

    return app.exec();

}
