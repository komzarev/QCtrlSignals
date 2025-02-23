INCLUDEPATH += $$PWD/src

DEFINES += QCTRLSIGNALS_INCLUDE_SOURCE

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
