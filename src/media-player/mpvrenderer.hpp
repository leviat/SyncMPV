#ifndef MPVRENDERER_HPP
#define MPVRENDERER_HPP

#include "mpvrenderer.hpp"

//Qt
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickFramebufferObject>

//libmpv
#include <client.h>
#include <opengl_cb.h>
#include <qthelper.hpp>

namespace mplayer {

class MpvObject;

class MpvRenderer : public QQuickFramebufferObject::Renderer
{
    /*====================================================================*/
    /* VARIABLES                                                                                                    */
    /*====================================================================*/

    private:
    mpv::qt::Handle mpv;
    QQuickWindow *window;
    mpv_opengl_cb_context *mpv_gl;

    /*====================================================================*/
    /* FUNCTIONS                                                                                                   */
    /*====================================================================*/

    public:
    MpvRenderer(const MpvObject *obj);
    virtual ~MpvRenderer();
    void render();

    private:
    static void *get_proc_address(void *ctx, const char *name);

};

} // namespace mplayer
#endif // MPVRENDERER_HPP
