/*
 * mathengine.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: Vaughn Friesen
 */

#define _USE_MATH_DEFINES

#include <sstream>
#include <iomanip>
#include <screen/screen.h>
#include <limits>
#include "mathengine.hpp"

#define DEBUG_CLASS "MathEngine"
#include "debug.hpp"

using namespace mu;
using namespace std;

inline double cut(double value, double cutoff=1e-15) {
	return (abs(value) > cutoff) * value;
}

value_type *AddVariable(const char_type *varName, void *userData)
{
	(void)varName;

	QVector<value_type> *vec = (QVector<value_type> *)userData;
	vec->push_back(0);

	return &vec->back();
}

MathEngine::MathEngine() {
	lastEval = 0.0;

	DEBUG_PRINT("Setting parser variable factory");
	parser.SetVarFactory(AddVariable, &variables);

	DEBUG_PRINT("Defining parser constants");

	parser.DefineConst("pi", (double)CONST_PI);
	parser.DefineConst("e", (double)CONST_E);
}

MathEngine::~MathEngine() {

}

QString MathEngine::evaluate(QString input) {
	if (!futureHistory.isEmpty())
		futureHistory.clear();

	try
	{
		parser.DefineVar("Ans", &lastEval);
		parser.SetExpr(input.toLocal8Bit().data());

		lastEval = parser.Eval();

		stringstream s;
		s.precision(12);
		s << cut(lastEval);

		pastHistory.push(input);

		return QString::fromStdString(s.str());
	}
	catch (Parser::exception_type &e)
	{
		return QString::fromStdString(e.GetMsg());
	}

	return "Waaa...?";
}

bool MathEngine::canGetPrevious() {
	return (!pastHistory.isEmpty());
}

QString MathEngine::getPrevious() {
	if (!canGetPrevious())
		return "";

	QString ret = pastHistory.pop();

	futureHistory.push(ret);

	return ret;
}

bool MathEngine::canGetNext() {
	return (!futureHistory.isEmpty());
}

QString MathEngine::getNext() {
	if (!canGetNext())
		return "";

	QString ret = futureHistory.pop();

	pastHistory.push(ret);

	return ret;
}
