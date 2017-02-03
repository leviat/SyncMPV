#ifndef MPVEVENTEMITTER_HPP
#define MPVEVENTEMITTER_HPP

/*====================================================================*/
/* INCLUDES                                                                                                      */
/*====================================================================*/

//Qt
#include <qthread.h>

//libmpv
#ifdef _WIN32
    #include <client.h>
    #include <opengl_cb.h>
    #include <qthelper.hpp>
#else
    #include <mpv/client.h>
    #include <mpv/opengl_cb.h>
    #include <mpv/qthelper.hpp>
#endif

class MpvObject;

class MpvEventEmitter : public QThread
{
    Q_OBJECT

    /*====================================================================*/
    /* VARIABLES                                                                                                    */
    /*====================================================================*/

private:
    mpv::qt::Handle mpv;
    bool shutdown = false;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    MpvEventEmitter(mpv::qt::Handle mpv, MpvObject* mpvObject);
    void initiateShutdown();

private:
    void run();

signals:
    void playtimeChanged();
    void volumeChanged();
};

#endif // MPVEVENTEMITTER_HPP
