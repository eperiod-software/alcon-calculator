/*
 * pathdata.cpp
 *
 *  Created on: May 2, 2013
 *      Author: Vaughn Friesen
 */

#include "pathdata.hpp"
#include "displayworker.hpp"

#define DEBUG_CLASS "Path"
#include "debug.hpp"

using namespace std;
using namespace mu;

Path::Path() : count(0), start(0), end(0), funcOfY(false), input(0.0) {
	parser.DefineConst("pi", (double)M_PI);
	parser.DefineConst("e", (double)M_E);
}

Path &Path::operator=(const Path& other) {
	setEqn(other.eqn, other.funcOfY);
	count = other.count;
	start = other.start;
	end = other.end;
	params = other.params;

	return *this;
}

bool Path::setEqn(string eqn, bool funcOfY) {
	this->eqn = eqn;
	this->funcOfY = funcOfY;

	input = 0;

	parser.SetExpr(eqn);

	if (funcOfY) {
		parser.DefineVar("y", &input);

		try
		{
			parser.Eval();
			return true;
		}
		catch (Parser::exception_type &e)
		{
			return false;
		}
	}
	else {
		parser.DefineVar("x", &input);

		try
		{
			parser.Eval();
			return true;
		}
		catch (Parser::exception_type &e)
		{
			return false;
		}
	}

	return false;
}

void Path::evaluate(DisplayWorker *displayWorker, std::size_t count) {
	this->count = 0;

	if (eqn.length() == 0) {
		QMutexLocker locker(&mutex);
		params.clear();
		return;
	}

	vector<double> newParams(count);

	newParams.resize(count);

	// Compute the stuff from start to end
	//memset(errs, 0, sizeof(bool) * width); // Set all errors and infinity to false
	//memset(infs, 0, sizeof(bool) * width);

	double y;

	for (size_t x = 0; x < count; x++) {
		input = displayWorker->screenXToReal(x);

		try
		{
			y = parser.Eval();

			//if (abs(y) == std::numeric_limits<double>::infinity())
			//	infs[x] = true;
		}
		catch (Parser::exception_type &e)
		{
			y = 0.0;
			//errs[x] = true;
		}

		newParams.push_back(y);
	}

	QMutexLocker locker(&mutex);
	params = newParams;
	this->count = count;
}

const vector<double> &Path::getParams(int *init) {
	*init = 0;

	return params;
}

void Path::draw(DisplayWorker *displayWorker) {
	QMutexLocker locker(&mutex);

	// Do the drawing
	string drawing("Drawing path \"");
	drawing += eqn + "\"";
	DEBUG_PRINT(drawing.c_str());

	Q_UNUSED(displayWorker);
}

PathData::PathData() : numPaths(NUM_PATHS) { }

PathData::PathData(const PathData &copy) : numPaths(copy.numPaths) {
	for (int x = 0; x < numPaths; x++)
		paths[x] = copy.paths[x];
}
