#ifndef MPVEVENTEMITTER_HPP
#define MPVEVENTEMITTER_HPP

//Qt
#include <qthread.h>

//libmpv
#include <client.h>
#include <opengl_cb.h>
#include <qthelper.hpp>


namespace mplayer {

class MpvObject;

class MpvEventEmitter : public QThread
{
    Q_OBJECT

    /*====================================================================*/
    /* VARIABLES                                                          */
    /*====================================================================*/

private:
    mpv::qt::Handle mpv;
    bool shouldTerminate;

    /*====================================================================*/
    /* FUNCTIONS                                                          */
    /*====================================================================*/

public:
    MpvEventEmitter(mpv::qt::Handle mpv, MpvObject* mpvObject);
    void initiateShutdown();

private:
    void run();

signals:
    void playtimeChanged();
    void volumeChanged();
    void stateChanged();
    void mediumChanged();
    void pausedChanged();
};

} // namespace mplayer
#endif // MPVEVENTEMITTER_HPP
