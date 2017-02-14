QT += qml quick

CONFIG += console
CONFIG += c++11

RESOURCES += syncmpv.qrc

HEADERS += \
    mplayer/mpvobject.hpp \
    mplayer/mpvrenderer.hpp \
    mplayer/mpveventemitter.hpp \
    network/protocol.hpp \
    network/host.hpp \
    network/client.hpp

SOURCES += \
    mplayer/mpvobject.cpp \
    mplayer/mpvrenderer.cpp \
    main.cpp \
    mplayer/mpveventemitter.cpp \
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
