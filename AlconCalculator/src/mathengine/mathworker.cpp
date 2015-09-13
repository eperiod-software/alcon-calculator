/*
 * mathworker.cpp
 *
 *  Created on: Apr 30, 2013
 *      Author: Vaughn Friesen
 */

#include "mathworker.hpp"
#include "displayworker.hpp"

#define DEBUG_CLASS "MathWorker"
#include "debug.hpp"

using namespace std;

MathWorker::MathWorker(DisplayWorker *displayWorker) : WorkerBase(),
		displayWorker(displayWorker) {
	QMutexLocker locker(&mutex);
	eqns = new string[paths.numPaths];
	graphChanged = false;
	graphDomainRangeChanged = false;
}

MathWorker::~MathWorker() {
	QMutexLocker locker(&mutex);
	delete []eqns;
}

bool MathWorker::setEqn(int num, QString eqn, bool funcOfY) {
	if (num >= 0 && num < paths.numPaths) {
		QMutexLocker locker(&mutex);
		eqns[num] = eqn.toLocal8Bit().data();
		graphChanged = true;
		return paths.paths[num].setEqn(eqns[num], funcOfY);
	}

	return false;
}

void MathWorker::setGraphDomainRangeChanged() {
	graphDomainRangeChanged = true;
}

void MathWorker::beginProcess() {
	DEBUG_PRINT("Math worker thread started");
}

bool MathWorker::tryProcess() {
	// Check if the equations or domain/range have changed
	if (!graphChanged && !graphDomainRangeChanged)
		return false;

	// Process each worker thread
	for (int x = 0; x < paths.numPaths; x++) {
		paths.paths[x].evaluate(displayWorker, displayWorker->getWidth());
	}

	graphDomainRangeChanged = false;
	graphChanged = false;

	displayWorker->setPathDataDisplayable(new PathData(paths));

	return true;
}

void MathWorker::endProcess() {
	DEBUG_PRINT("Math worker thread finished");
}
