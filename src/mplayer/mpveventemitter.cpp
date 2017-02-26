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
    mpv_observe_property(mpv, 0, "size", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "length", MPV_FORMAT_NONE);

    // when the audio is initially played, mpv doesn't emit an event for the change of ao-volume
    // we also listen to a change of the codec instead to catch the first time audio is played
    mpv_observe_property(mpv, 0, "audio-codec", MPV_FORMAT_NONE);

    connect(this, &MpvEventEmitter::playtimeChanged, mpvObject, &MpvObject::playtimeChanged, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::volumeChanged, mpvObject, &MpvObject::volumeChanged, Qt::QueuedConnection);

    connect(this, &MpvEventEmitter::stateChanged, mpvObject, &MpvObject::updateState, Qt::QueuedConnection);


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
        }

        // Sync relevant changes
        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "time-pos" || eventName == "core-idle")
                emit stateChanged();
        }

        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "length" || eventName == "size")
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
