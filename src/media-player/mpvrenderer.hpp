#ifndef MPVRENDERER_HPP
#define MPVRENDERER_HPP

/*====================================================================*/
/* INCLUDES                                                                                                      */
/*====================================================================*/

#include "mpvrenderer.hpp"

//Qt
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickFramebufferObject>

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




/*====================================================================*/
/* FORWARD DECLARATIONS  (GLOBAL NAMESPACE)                                      */
/*====================================================================*/

class MpvObject;

/*====================================================================*/
/* CLASS DEFINITIONS                                                                                      */
/*====================================================================*/

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

#endif // MPVRENDERER_HPP
