QT += qml quick

CONFIG += static
CONFIG += console

RESOURCES += syncmpv.qrc

HEADERS += \
    media-player/mpvobject.hpp \
    media-player/mpvrenderer.hpp \
    media-player/mpveventemitter.hpp \
    network/protocol.hpp \
    network/host.hpp \
    network/client.hpp

SOURCES += \
    media-player/mpvobject.cpp \
    media-player/mpvrenderer.cpp \
    main.cpp \
    media-player/mpveventemitter.cpp \
    network/protocol.cpp \
    network/host.cpp \
    network/client.cpp

DISTFILES += \
    view/main.qml \
    ../.travis.yml \
    ../.gitignore \
    ../README.md


win32: LIBS += -L$$PWD/../libmpv/ -lmpv

INCLUDEPATH += $$PWD/../libmpv/include
DEPENDPATH += $$PWD/../libmpv/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/mpv.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/libmpv.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += mpv
