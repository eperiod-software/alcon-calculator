/*
 * mathengine.hpp
 *
 *  Created on: Apr 18, 2013
 *      Author: Vaughn Friesen
 */

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QStack>
#include <screen/screen.h>
#include <QThread>
#include "muparser/muParser.h"

#ifndef MATHENGINE_HPP_
#define MATHENGINE_HPP_

#define CONST_PI  3.141592653589793238462643
#define CONST_E   2.718281828459045235360287

class MathEngine : public QObject {
	Q_OBJECT
	Q_PROPERTY(int canGetPrevious READ canGetPrevious)
	Q_PROPERTY(int canGetNext READ canGetNext)

public:
	MathEngine();
	virtual ~MathEngine();

	Q_INVOKABLE QString evaluate(QString input);

	bool canGetPrevious();
	Q_INVOKABLE QString getPrevious();
	bool canGetNext();
	Q_INVOKABLE QString getNext();

private:
	QStack<QString> pastHistory;
	QStack<QString> futureHistory;
	double lastEval;
	mu::Parser parser;

	QVector<mu::value_type> variables;

	QString eqn;
};

#endif /* MATHENGINE_HPP_ */
