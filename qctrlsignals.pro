QT -= gui

TEMPLATE = lib

CONFIG += c++11

contains(DEFINES, STATIC_LIBS_BUILD) {
    CONFIG   += staticlib
}
else {
    CONFIG   += lib
}

INCLUDEPATH += $$PWD/src

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QCTRLSIGNALS_LIBRARY

HEADERS += $$INCLUDEPATH/qctrlsignalhandler.h \
           $$INCLUDEPATH/qctrlsignal_global.h \
           $$INCLUDEPATH/qctrlsignalhandler_p.h


SOURCES += $$INCLUDEPATH/qctrlsignalhandler.cpp

win32 {
        HEADERS += $$INCLUDEPATH/qctrlsignalhandler_win.h
        SOURCES += $$INCLUDEPATH/qctrlsignalhandler_win.cpp
}

unix {
        HEADERS += $$INCLUDEPATH/qctrlsignalhandler_unix.h
        SOURCES += $$INCLUDEPATH/qctrlsignalhandler_unix.cpp
}

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    qctrlsignals.pri
