#ifndef QCTRLSIGNAL_GLOBAL_H
#define QCTRLSIGNAL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QCTRLSIGNALS_LIBRARY)
#  define Q_CTRL_SIGNALS_EXPORT Q_DECL_EXPORT
#elif defined(QCTRLSIGNALS_INCLUDE_SOURCE)
#  define Q_CTRL_SIGNALS_EXPORT
#else
#  define Q_CTRL_SIGNALS_EXPORT Q_DECL_IMPORT
#endif

#endif // QCTRLSIGNAL_GLOBAL_H
