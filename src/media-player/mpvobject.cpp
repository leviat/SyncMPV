#include "mpvobject.hpp"

//Qt Core
#include <QObject>
#include <QtGlobal>
#include <QOpenGLContext>
#include <QGuiApplication>

//Qt GUI
#include <QtGui/QOpenGLFramebufferObject>

//Qt Quick
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>



MpvObject::MpvObject(QQuickItem * parent)
    : QQuickFramebufferObject(parent), mpv_gl(0)
{
    mpv = mpv::qt::Handle::FromRawHandle(mpv_create());
    if (!mpv)
        throw std::runtime_error("could not create mpv context");

    mpv_set_option_string(mpv, "terminal", "yes");
    mpv_set_option_string(mpv, "msg-level", "all=info");
    mpv_set_option_string(mpv, "ytdl", "yes");

    if (mpv_initialize(mpv) < 0)
        throw std::runtime_error("could not initialize mpv context");

    // Make use of the MPV_SUB_API_OPENGL_CB API.
    mpv::qt::set_option_variant(mpv, "vo", "opengl-cb");

    mpv::qt::set_option_variant(mpv, "hwdec", "auto");

    // Setup the callback that will make QtQuick update and redraw if there
    // is a new video frame. Use a queued connection: this makes sure the
    // doUpdate() function is run on the GUI thread.
    mpv_gl = (mpv_opengl_cb_context *)mpv_get_sub_api(mpv, MPV_SUB_API_OPENGL_CB);
    if (!mpv_gl)
        throw std::runtime_error("OpenGL not compiled in");
    mpv_opengl_cb_set_update_callback(mpv_gl, MpvObject::on_update, (void *)this);
    connect(this, &MpvObject::onUpdate, this, &MpvObject::doUpdate,
            Qt::QueuedConnection);

    eventEmitter = new MpvEventEmitter(mpv, this);
    eventEmitter->start();
}

MpvObject::~MpvObject()
{
    eventEmitter->initiateShutdown();
    eventEmitter->wait(1000);
    if (mpv_gl)
        mpv_opengl_cb_set_update_callback(mpv_gl, NULL, NULL);
}

void MpvObject::on_update(void *ctx)
{
    MpvObject *self = (MpvObject *)ctx;
    emit self->onUpdate();
}

// connected to onUpdate(); signal makes sure it runs on the GUI thread
void MpvObject::doUpdate()
{
    update();
}

void MpvObject::command(const QVariant& params)
{
    mpv::qt::command_variant(mpv, params);
}

bool isError(QVariant property_result) {
    return QString(property_result.typeName()).compare("mpv::qt::ErrorReturn") == 0;
}

double MpvObject::getPercentPlaytime() {

    QVariant percent_pos = mpv::qt::get_property(mpv, "percent-pos");

    if (isError(percent_pos))
        return 0.0;

     return percent_pos.toDouble();

}

QString MpvObject::getPlaytime() {
    QVariant time_pos = mpv::qt::get_property(mpv, "time-pos");

    if (isError(time_pos))
        return "hh:mm:ss";

    int secs = time_pos.toInt() % 60;
    int mins = time_pos.toInt() / 60 % 60;
    int hours = time_pos.toInt() / 3600;

    QString ss = QString("%1").arg(secs, 2, 10, QChar('0'));
    QString mm = QString("%1:").arg(mins, 2, 10, QChar('0'));
    QString hh = QString("%1:").arg(hours, 2, 10, QChar('0'));

    return hh.append(mm).append(ss);

}

double MpvObject::getVolume() {
    QVariant volume = mpv::qt::get_property(mpv, "ao-volume");

    if (isError(volume))
        return 100.00;

    return volume.toDouble();

}

void MpvObject::setProperty(const QString& name, const QVariant& value)
{
    mpv::qt::set_property_variant(mpv, name, value);
}
