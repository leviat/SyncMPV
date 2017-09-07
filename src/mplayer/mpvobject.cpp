#include "mpvobject.hpp"

#include <stdexcept>

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

namespace mplayer {

/*!
    \class mplayer::MpvObject
    \inmodule mplayer
    \brief The MpvObject acts as the controller for the GUI interface

    The MpvObject provides basic methods to pass commands to the underlying mpv thread.
    Mpv's properties can also be obtained from this class.
*/

/*!
  \fn void MpvObject::onUpdate()
  \brief Is emitted when a new frame has been rendered.
*/

/*!
    \property MpvObject::playtime
    \brief The current playtime
*/

/*!
    \property MpvObject::percentPlaytime
    \brief The current playtime in percent
*/

/*!
    \property MpvObject::volume
    \brief The current volume
*/

/*!
    \enum playState

    \value PLAY
           The player is currently playing..
    \value PAUSE
           The player is paused.
*/

/*!
    \property MpvObject::bufferProgress
    \brief The current buffering progress in percent from [0.00 - 100.0]
*/

/*!
    \property MpvObject::paused
    \brief If the player is paused.
*/


/*!
    \brief Constructs the underlying mpv player.

    The constructor sets some default properties for the mpv player.
    It also sets the callback for the rendering methods for the framebuffer \a parent.
    Furthermore we deploy an MpvEventEmitter thread which handles the mpv's events for us.
*/
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
    waitingForBuffer = false;

}

/*!
    \brief Basic destructor
 */
MpvObject::~MpvObject()
{
    eventEmitter->initiateShutdown();
    eventEmitter->wait(1000);
    delete eventEmitter;

    if (mpv_gl)
        mpv_opengl_cb_set_update_callback(mpv_gl, NULL, NULL);
}

/*!
    \brief Returns a QQuickFramebufferObject into which the video frames are rendered.
 */
QQuickFramebufferObject::Renderer *MpvObject::createRenderer() const
{
    window()->setPersistentOpenGLContext(true);
    window()->setPersistentSceneGraph(true);
    return new MpvRenderer(this);
}

/*!
    \brief This emits the onUpdate signal for the given mpv \a ctx.
 */
void MpvObject::on_update(void *ctx)
{
    MpvObject *self = (MpvObject *)ctx;
    emit self->onUpdate();
}

/*!
    \brief Updates the MpvObject

    Connected to onUpdate(); signal makes sure it runs on the GUI thread
*/
void MpvObject::doUpdate()
{
    update();
}

/*!
    \brief MpvObject::command passes the commands as \a params to the mpv-player.
*/
void MpvObject::command(const QVariant params)
{
    mpv::qt::command_variant(mpv, params);
}

/*!
    \brief isError returns true if the \a property_result is an error.
*/
bool isError(QVariant property_result) {
    return QString(property_result.typeName()).compare("mpv::qt::ErrorReturn") == 0;
}

/*!
    \brief The playtime in percent
    Returns the playtime in percent (0-100%). If no media is available, this returns 0.
*/
double MpvObject::percentPlaytime() {

    QVariant percent_pos = mpv::qt::get_property(mpv, "percent-pos");

    if (isError(percent_pos))
        return 0.0;

     return percent_pos.toDouble();

}

/*!
    \brief The playtime as [hh:mm:ss]
    Returns the playtime as a QString formatted as [hh:mm:ss]
    If there is no media available, this returns the string "hh:mm:ss".
*/
QString MpvObject::playtime() {
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

/*!
    \brief The volume in percent (0-100%).
    If no audio media is available, this returns 100.00
*/
double MpvObject::volume() {
    QVariant volume = mpv::qt::get_property(mpv, "ao-volume");

    if (isError(volume))
        return 100.00;

    return volume.toDouble();

}

/*!
    \brief Sets the  mpv \a property to the \a value.
*/
void MpvObject::setProperty(const QString property, const QVariant value)
{
    mpv::qt::set_property_variant(mpv, property, value);
}

/*!
    \brief Retrieves the media player's state from the mpv thread and stores it in an internal data structure.
*/
mplayer::state MpvObject::updateState() {
    mplayer::state state = {PAUSE, 0};

    QVariant coreIdleProperty = mpv::qt::get_property(mpv, "core-idle");

    if (coreIdleProperty.toBool() == false)
        state.playState = PLAY;
    else
        state.playState = PAUSE;

    QVariant timeProperty = mpv::qt::get_property(mpv, "time-pos");
    if (isError(timeProperty))
        state.playTime = 0;
    else
        state.playTime = timeProperty.toDouble();

    QVariant demuxerCacheProperty = mpv::qt::get_property(mpv, "demuxer-cache-duration");
    if (isError(demuxerCacheProperty))
        state.demuxerCache = 0;
    else
        state.demuxerCache = demuxerCacheProperty.toDouble();

    QVariant additionalCacheProperty = mpv::qt::get_property(mpv, "cache-used");
    if (isError(additionalCacheProperty))
        state.additionalCache = 0;
    else
        state.additionalCache = additionalCacheProperty.toDouble();

    emit stateChanged(state);
    return state;
}

/*!
 * \brief Returns information about the current medium and emits a signal that information might have changed.
 */

mediumInfo MpvObject::updateMediumInfo() {
    mediumInfo info;
    QVariant lengthProperty = mpv::qt::get_property(mpv, "duration");
    QVariant sizeProperty = mpv::qt::get_property(mpv, "file-size");

    if (isError(lengthProperty))
        info.duration = 0;
    else
        info.duration = lengthProperty.toUInt();

    if (isError(sizeProperty))
        info.fileSize = 0;
    else
        info.fileSize = sizeProperty.toDouble() / 1024;

    emit mediumChanged(info);

    return info;
}

/*!
 * \brief Returns if the player is paused.
 */

bool MpvObject::paused() {
    QVariant pausedProperty = mpv::qt::get_property(mpv, "pause");

    if (isError(pausedProperty))
        return true;
    else
        return pausedProperty.toBool();
}

/*!
 * \brief Returns how much of the remaining medium has been buffered in percent from [0.00 - 100.00]
 */

double MpvObject::bufferProgress() {
    mediumInfo medium = updateMediumInfo();
    mplayer::state player = updateState();

    double remainingkB = (1 - player.playTime / medium.duration) * medium.fileSize;
    double progress = player.demuxerCache / (medium.duration - player.playTime); //demuxer
    progress += player.additionalCache / remainingkB;

    return progress * 100;
}

/*!
 * \brief Toggles the player from pause to unpause and vice-versa.
 */

void MpvObject::switchPause() {
    QVariant pausedProperty = mpv::qt::get_property(mpv, "pause");

    if (isError(pausedProperty))
        return;

    setProperty("pause", !pausedProperty.toBool());
}

/*!
 * \brief Tells the player to pause and wait for sufficient buffer.
 */

void MpvObject::waitForBuffer() {
    waitingForBuffer = true;
    setProperty("pause", true);
}

/*!
 * \brief Resumes playing after sufficient buffering.
 */

void MpvObject::playAfterBuffering() {
    waitingForBuffer = false;
    setProperty("pause", false);
}

// Misc. documentation

/*!
    \fn void MpvObject::playtimeChanged()
    \brief Is emitted when the playtime has changed.
*/

/*!
    \fn void MpvObject::volumeChanged()
    \brief Is emitted when the volume has changed.
*/

/*!
 * \fn void MpvObject::stateChanged(mplayer::state newState)
 * \brief Is emitted when the player state has changed to a \a newState that requires re-sync to all clients.
 */

/*!
 * \fn void MpvObject::mediumChanged(mplayer::mediumInfo mediumInfo)
 * \brief Is emitted when another medium with is loaded. Provides \a mediumInfo about the new medium.
 */

/*!
 * \fn void MpvObject::pausedChanged()
 * \brief Is emitted when the player is paused/unpaused.
 */

/*!
 * \fn void MpvObject::bufferProgressChanged()
 * \brief Is emitted when the buffer progress changes.
 */

} //namespace mplayer

