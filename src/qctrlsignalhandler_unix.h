#ifndef QCTRLSIGNALHANDLER_UNIX_H
#define QCTRLSIGNALHANDLER_UNIX_H

#include "qctrlsignalhandler_p.h"

#include <QSocketNotifier>
#include <QObject>

class QCtrlSignalHandlerUnix : public QObject, public QCtrlSignalHandlerPrivate
{
	Q_OBJECT

public:
	QCtrlSignalHandlerUnix(QCtrlSignalHandler *q);

	bool registerSignal(int signal) override;
	bool unregisterSignal(int signal) override;
	void changeAutoQuitMode(bool enabled) override;
	QReadWriteLock *lock() const override;
    void callPreviousHandler(int sig) override;
private Q_SLOTS:
	void socketNotifyTriggerd(int socket);

private:
	QSocketNotifier *socketNotifier = nullptr;

	bool isAutoQuitRegistered(int signal) const;
	bool updateSignalHandler(int signal, bool active);

	static int sockpair[2];
	static const QVector<int> shutSignals;

	static void unixSignalHandler(int signal);
    typedef	void (*pHandler)(int);
    QMap<int,pHandler> prevHandlers_;

};

#endif // QCTRLSIGNALHANDLER_UNIX_H
