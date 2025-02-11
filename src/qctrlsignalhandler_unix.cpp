#include "qctrlsignalhandler_unix.h"
#include <QCoreApplication>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>

int QCtrlSignalHandlerUnix::sockpair[2];
const QVector<int> QCtrlSignalHandlerUnix::shutSignals = {SIGINT, SIGTERM, SIGQUIT, SIGHUP};

QCtrlSignalHandlerPrivate *QCtrlSignalHandlerPrivate::createInstance(QCtrlSignalHandler *q_ptr)
{
	return new QCtrlSignalHandlerUnix{q_ptr};
}

QCtrlSignalHandlerUnix::QCtrlSignalHandlerUnix(QCtrlSignalHandler *q_ptr) :
	QObject{},
	QCtrlSignalHandlerPrivate{q_ptr}
{
	if(::socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair) == 0) {
		socketNotifier = new QSocketNotifier{sockpair[1], QSocketNotifier::Read, this};
		connect(socketNotifier, &QSocketNotifier::activated,
				this, &QCtrlSignalHandlerUnix::socketNotifyTriggerd);
		socketNotifier->setEnabled(true);
	} else {
		qCWarning(logQCtrlSignals).noquote() << "Failed to create socket pair with error:"
											 << qt_error_string();
	}
}

bool QCtrlSignalHandlerUnix::registerSignal(int signal)
{
	if(isAutoQuitRegistered(signal))
		return true;
	else
		return updateSignalHandler(signal, true);
}

bool QCtrlSignalHandlerUnix::unregisterSignal(int signal)
{
	if(isAutoQuitRegistered(signal))
		return true;
	else
		return updateSignalHandler(signal, false);
}

void QCtrlSignalHandlerUnix::changeAutoQuitMode(bool enabled)
{
	for(auto sig : shutSignals) {
		if(!activeSignals.contains(sig))
			updateSignalHandler(sig, enabled);
	}
}

QReadWriteLock *QCtrlSignalHandlerUnix::lock() const
{
    return nullptr;//no locks needed on unix
}

void QCtrlSignalHandlerUnix::callPreviousHandler(int sig)
{
    if(prevHandlers_.contains(sig)){
        auto handl = prevHandlers_[sig];
        if(handl == SIG_DFL){
            signal(sig, SIG_DFL);
            raise(sig);
        } else if(handl == SIG_IGN){
            return;
        } else {
            handl(sig);
        }
    }
}

void QCtrlSignalHandlerUnix::socketNotifyTriggerd(int socket)
{
	int signal;
	if(::read(socket, &signal, sizeof(int)) == sizeof(int)) {
		if(!reportSignalTriggered(signal) &&
		   isAutoQuitRegistered(signal))
			qApp->quit();
    } else {
		qCWarning(logQCtrlSignals) << "Failed to read signal from socket pair";
    }
}

bool QCtrlSignalHandlerUnix::isAutoQuitRegistered(int signal) const
{
	if(autoQuit)
		return shutSignals.contains(signal);
	else
		return false;
}

bool QCtrlSignalHandlerUnix::updateSignalHandler(int sig, bool active)
{
    if(active)  {
        auto prevHandler = signal(sig, &QCtrlSignalHandlerUnix::unixSignalHandler);
        if(prevHandler != QCtrlSignalHandlerUnix::unixSignalHandler){
            prevHandlers_.insert(sig,prevHandler);
        }

        signal(sig, QCtrlSignalHandlerUnix::unixSignalHandler);
    } else {
        signal(sig, SIG_DFL);
    }

    return true;
}

void QCtrlSignalHandlerUnix::unixSignalHandler(int signal)
{
	const auto wr = ::write(sockpair[0], &signal, sizeof(int));
    Q_UNUSED(wr)
}
