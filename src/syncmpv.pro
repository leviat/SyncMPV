QT += qml quick

CONFIG += static

RESOURCES += syncmpv.qrc

HEADERS += \
    media-player/mpvobject.hpp \
    media-player/mpvrenderer.hpp \
    main.h \
    media-player/mpveventemitter.hpp

SOURCES += \
    media-player/mpvobject.cpp \
    media-player/mpvrenderer.cpp \
    main.cpp \
    media-player/mpveventemitter.cpp

DISTFILES += \
    view/main.qml \
    ../.gitlab-ci.yml \
    ../.gitignore


win32: LIBS += -L$$PWD/../libmpv/ -lmpv

INCLUDEPATH += $$PWD/../libmpv/include
DEPENDPATH += $$PWD/../libmpv/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/mpv.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/libmpv.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += mpv
