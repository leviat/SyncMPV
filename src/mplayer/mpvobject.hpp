#ifndef MPVOBJECT_HPP
#define MPVOBJECT_HPP

#include <mplayer/mpvrenderer.hpp>
#include <mplayer/mpveventemitter.hpp>

//Qt
#include <QtQuick/QQuickFramebufferObject>
#include <QMutex>

//libmpv
#include <client.h>
#include <opengl_cb.h>
#include <qthelper.hpp>

namespace mplayer {

enum playState {
    PLAY,
    PAUSE,
    BUFFERING //deprecated
};

struct state {
        mplayer::playState playState;
        double playTime;
        double demuxerCache; // in seconds
        double additionalCache; // in kB
};

struct mediumInfo {
    uint duration; // in s, length property
    double fileSize; // in kilobytes, size property (turned to kB)
};

/*====================================================================*/
/* CLASS DEFINITIONS                                                  */
/*====================================================================*/

class MpvObject : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString playtime READ playtime NOTIFY playtimeChanged)
    Q_PROPERTY(double percentPlaytime READ percentPlaytime NOTIFY playtimeChanged)
    Q_PROPERTY(double bufferProgress READ bufferProgress NOTIFY bufferProgressChanged)
    Q_PROPERTY(double volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(bool paused READ paused NOTIFY pausedChanged)

    friend class MpvRenderer;

    /*====================================================================*/
    /* VARIABLES                                                          */
    /*====================================================================*/

private:
    mpv::qt::Handle mpv;
    mpv_opengl_cb_context *mpv_gl;
    MpvEventEmitter* eventEmitter;
    bool waitingForBuffer;

    /*====================================================================*/
    /* FUNCTIONS                                                          */
    /*====================================================================*/

public:
    MpvObject(QQuickItem * parent = 0);
    virtual ~MpvObject();
    virtual Renderer *createRenderer() const;
    QString playtime();
    double volume();
    double percentPlaytime();
    double bufferProgress();
    bool paused();
    mplayer::state state();

private:
    static void on_update(void *ctx);

signals:
    void onUpdate();
    void playtimeChanged();
    void volumeChanged();
    void stateChanged(mplayer::state newState);
    void mediumChanged(mplayer::mediumInfo mediumInfo);
    void pausedChanged();
    void bufferProgressChanged();

public slots:
    void command(const QVariant params);
    void setProperty(const QString name, const QVariant value);
    mplayer::state updateState();
    mediumInfo updateMediumInfo();
    void switchPause();
    void waitForBuffer();
    void playAfterBuffering();

private slots:
    void doUpdate();

};

} // namespace mplayer
#endif // MPVOBJECT_HPP


