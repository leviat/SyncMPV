QT += qml quick

CONFIG += console
CONFIG += c++11

RESOURCES += syncmpv.qrc

HEADERS += \
    mplayer/mpvobject.hpp \
    mplayer/mpvrenderer.hpp \
    mplayer/mpveventemitter.hpp \
    network/hostsocket.hpp \
    network/clientsocket.hpp \
    sync/protocol.hpp \
    sync/host.hpp \
    sync/client.hpp \
    sync/clientinfo.hpp \
    sync/clientinfomodel.hpp

SOURCES += \
    mplayer/mpvobject.cpp \
    mplayer/mpvrenderer.cpp \
    main.cpp \
    mplayer/mpveventemitter.cpp \
    network/clientsocket.cpp \
    network/hostsocket.cpp \
    sync/protocol.cpp \
    sync/host.cpp \
    sync/client.cpp \
    sync/clientinfo.cpp \
    sync/clientinfomodel.cpp

DISTFILES += \
    view/main.qml \
    ../.travis.yml \
    ../.gitignore \
    ../README.md \
    view/Sidebar.qml \
    view/HostTab.qml \
    view/ClientTab.qml

win32: LIBS += -L$$PWD/../libmpv/ -lmpv

INCLUDEPATH += $$PWD/../libmpv/include
DEPENDPATH += $$PWD/../libmpv/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/mpv.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libmpv/libmpv.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += mpv
