#include "mpveventemitter.hpp"
#include <media-player/mpvobject.hpp>

MpvEventEmitter::MpvEventEmitter(mpv::qt::Handle mpv, MpvObject *mpvObject)
{
    this->mpv = mpv;
    this->shutdown = false;

    // Setup callbacks for qt property signals
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_NONE);
    mpv_observe_property(mpv, 0, "ao-volume", MPV_FORMAT_NONE);

    // when the audio is initially played, mpv doesn't emit an event for the change of ao-volume
    // we also listen to a change of the codec instead to catch the first time audio is played
    mpv_observe_property(mpv, 0, "audio-codec", MPV_FORMAT_NONE);

    connect(this, &MpvEventEmitter::playtimeChanged, mpvObject, &MpvObject::playtimeChanged, Qt::QueuedConnection);
    connect(this, &MpvEventEmitter::volumeChanged, mpvObject, &MpvObject::volumeChanged, Qt::QueuedConnection);

}

void MpvEventEmitter::run(){
    while (!shutdown) {
        mpv_event *event = mpv_wait_event(mpv, 0.5);

        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            QString eventName =  QString(reinterpret_cast<mpv_event_property*>(event->data)->name);

            if (eventName == "time-pos")
                emit playtimeChanged();
            else if (eventName == "ao-volume" || eventName == "audio-codec")
                emit volumeChanged();
        }
    }
}

void MpvEventEmitter::initiateShutdown() {
    shutdown = true;
}
