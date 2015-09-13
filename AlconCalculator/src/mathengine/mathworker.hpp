/*
 * mathworker.hpp
 *
 *  Created on: Apr 30, 2013
 *      Author: Vaughn Friesen
 */

#include <QThread>
#include <QMutex>
#include <QString>
#include "workerbase.hpp"
#include "pathdata.hpp"

#ifndef MATHWORKER_HPP_
#define MATHWORKER_HPP_

class DisplayWorker;

class MathWorker : public WorkerBase {
	Q_OBJECT

public:
	MathWorker(DisplayWorker *displayWorker);
	virtual ~MathWorker();

	Q_INVOKABLE bool setEqn(int num, QString eqn, bool funcOfY);

	void setGraphDomainRangeChanged();

protected:
	void beginProcess();
	bool tryProcess();
	void endProcess();

	DisplayWorker *displayWorker;
	PathData paths;

	mutable QMutex mutex;
	std::string *eqns;
	volatile bool graphChanged, graphDomainRangeChanged;
};

#endif /* MATHWORKER_HPP_ */
