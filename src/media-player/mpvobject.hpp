#ifndef MPVOBJECT_HPP
#define MPVOBJECT_HPP

/*====================================================================*/
/* INCLUDES                                                                                                      */
/*====================================================================*/

#include <media-player/mpvrenderer.hpp>
#include <media-player/mpveventemitter.hpp>

//Qt
#include <QtQuick/QQuickFramebufferObject>

//libmpv
#include <client.h>
#include <opengl_cb.h>
#include <qthelper.hpp>

namespace mplayer {

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

private:
    static void on_update(void *ctx);

signals:
    void onUpdate();
    void playtimeChanged();
    void volumeChanged();

public slots:
    void command(const QVariant& params);
    void setProperty(const QString& name, const QVariant& value);

private slots:
    void doUpdate();

};

} // namespace mplayer
#endif // MPVOBJECT_HPP


