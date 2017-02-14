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
    BUFFERING
};

struct state {
        mplayer::playState playState;
        double playTime;
};

/*====================================================================*/
/* CLASS DEFINITIONS                                                                                      */
/*====================================================================*/

class MpvObject : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString playtime READ playtime NOTIFY playtimeChanged)
    Q_PROPERTY(double percentPlaytime READ percentPlaytime NOTIFY playtimeChanged)
    Q_PROPERTY(double volume READ volume NOTIFY volumeChanged)

    friend class MpvRenderer;

    /*====================================================================*/
    /* VARIABLES                                                                                                    */
    /*====================================================================*/

private:
    mpv::qt::Handle mpv;
    mpv_opengl_cb_context *mpv_gl;
    MpvEventEmitter* eventEmitter;
    QMutex stateMutex;
    mplayer::state m_state;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

public:
    MpvObject(QQuickItem * parent = 0);
    virtual ~MpvObject();
    virtual Renderer *createRenderer() const;
    QString playtime();
    double volume();
    double percentPlaytime();
    mplayer::state state();

private:
    static void on_update(void *ctx);

signals:
    void onUpdate();
    void playtimeChanged();
    void volumeChanged();
    void stateChanged();

public slots:
    void command(const QVariant& params);
    void setProperty(const QString& name, const QVariant& value);
    void updateState();

private slots:
    void doUpdate();

};

} // namespace mplayer
#endif // MPVOBJECT_HPP


