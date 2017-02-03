#ifndef MPVEVENTEMITTER_HPP
#define MPVEVENTEMITTER_HPP

/*====================================================================*/
/* INCLUDES                                                                                                      */
/*====================================================================*/

//Qt
#include <qthread.h>

//libmpv
#include <client.h>
#include <opengl_cb.h>
#include <qthelper.hpp>


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
