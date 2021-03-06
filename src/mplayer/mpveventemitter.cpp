#include "mpveventemitter.hpp"
#include <mplayer/mpvobject.hpp>

namespace mplayer{

/*!
    \class mplayer::MpvEventEmitter
    \inmodule mplayer

    \brief Polls specific events from the mpv event queue and feeds them into the signal-slot system of Qt.
*/

/*!
    \fn MpvEventEmitter::MpvEventEmitter(mpv::qt::Handle mpv, MpvObject *mpvObject)
    \brief Constructs a QThread which will forward the mpv events to the signal-slot system.

    We have to provide it a handle to the underlying \a mpv thread so it can poll the events.
    It also needs the \a mpvObject to which it will forward the appropriate signals.
*/
MpvEventEmitter::MpvEventEmitter(mpv::qt::Handle mpv, MpvObject *mpvObject)
{
    this->mpv = mpv;
    this->shouldTerminate = false;

    // Setup callbacks for qt property signals
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "ao-volume", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "core-idle", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "file-size", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "demuxer-cache-duration", MPV_FORMAT_NONE);


    // when the audio is initially played, mpv doesn't emit an event for the change of ao-volume
    // we also listen to a change of the codec instead to catch the first time audio is played
    mpv_observe_property(mpv, 0, "audio-codec", MPV_FORMAT_NONE);

    connect(this, &MpvEventEmitter::playtimeChanged, mpvObject, &MpvObject::playtimeChanged, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::volumeChanged, mpvObject, &MpvObject::volumeChanged, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::pausedChanged, mpvObject, &MpvObject::pausedChanged, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::stateChanged, mpvObject, &MpvObject::updateState, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::mediumChanged, mpvObject, &MpvObject::updateMediumInfo, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::bufferChanged, mpvObject, &MpvObject::bufferProgressChanged, Qt::QueuedConnection);


}

/*!
    \brief Polls mpv events and forwards them to the signal-slot system.
    Can be terminated with initiateShutdown().
*/
void MpvEventEmitter::run(){
    while (!shouldTerminate) {
        mpv_event *event = mpv_wait_event(mpv, 0.5);

        // Qt UI relevant changes
        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "time-pos")
                emit playtimeChanged();
            else if (eventName == "ao-volume" || eventName == "audio-codec")
                emit volumeChanged();
            else if (eventName == "pause")
                emit pausedChanged();
            else if (eventName == "demuxer-cache-duration")
                emit bufferChanged();
        }

        // Sync relevant changes
        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "time-pos" || eventName == "core-idle")
                emit stateChanged();
        }

        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "file-size")
                emit mediumChanged();
        }
    }
}

/*!
    \brief Sets the thread into shutdown mode. It will terminate as soon as it finishes processing the current event.
*/
void MpvEventEmitter::initiateShutdown() {
    shouldTerminate = true;
}

} //namespace mplayer

// Misc. documentation

/*!
    \fn void MpvEventEmitter::playtimeChanged()
    \brief Is emitted when the playtime has changed.
*/

/*!
    \fn void MpvEventEmitter::volumeChanged()
    \brief Is emitted when the volume has changed.
*/

/*!
 * \fn void MpvEventEmitter::stateChanged()
 * \brief Is emitted when the player state has changed in a way that requires re-sync to all clients.
 */

/*!
 * \fn void MpvEventEmitter::mediumChanged()
 * \brief Is emitted when another medium is loaded.
 */

/*!
 * \fn void MpvEventEmitter::pausedChanged()
 * \brief Is emitted when the player is paused/unpaused.
 */

/*!
 * \fn void MpvEventEmitter::bufferChanged()
 * \brief Is emitted when the buffer progress changes.
 */
