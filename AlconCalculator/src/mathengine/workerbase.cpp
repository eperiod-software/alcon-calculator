/*
 * workerbase.cpp
 *
 *  Created on: May 2, 2013
 *      Author: Vaughn Friesen
 */

#include <QThread>
#include "workerbase.hpp"

#define DEBUG_CLASS "WorkerBase"
#include "debug.hpp"

class Sleeper : public QThread {
public:
   void usleep(int usec) { QThread::usleep(usec); }
};

static Sleeper slp;

WorkerBase::WorkerBase() : activeSleep(FPS_TO_USEC(ACTIVE_FPS)), passiveSleep(FPS_TO_USEC(PASSIVE_FPS)) {
	done = false;
}

WorkerBase::~WorkerBase() { }

void WorkerBase::setDone() {
	this->done = true;
}

void WorkerBase::process() {
    beginProcess();

    while (!isDone()) {
    	if (tryProcess()) {
    		usleep(activeSleep);
    	}
    	else {
    		usleep(passiveSleep);
    	}
    }

    endProcess();

    emit finished();
}

void WorkerBase::usleep(int usec) {
	slp.usleep(usec);
}
