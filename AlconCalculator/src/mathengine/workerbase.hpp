/*
 * workerbase.hpp
 *
 *  Created on: May 2, 2013
 *      Author: Vaughn Friesen
 */

#include <QObject>

#ifndef WORKERBASE_HPP_
#define WORKERBASE_HPP_

#ifndef ACTIVE_FPS
#define ACTIVE_FPS 45
#endif
#ifndef PASSIVE_FPS
#define PASSIVE_FPS 10
#endif

#define FPS_TO_USEC(x) ((int)(1.0f / (float)x * 1000.0f * 1000.0f))

class WorkerBase : public QObject {
	Q_OBJECT

public:
	WorkerBase();
	virtual ~WorkerBase();

	void setDone();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

protected:
	volatile bool done;
	inline bool isDone() { return done; }

	const int activeSleep, passiveSleep;

	void usleep(int usec);

	// These need to be overridden
	virtual void beginProcess() = 0;
	virtual bool tryProcess() = 0;
	virtual void endProcess() = 0;
};

#endif /* WORKERBASE_HPP_ */
