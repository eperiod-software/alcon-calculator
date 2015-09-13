/*
 * pathdata.hpp
 *
 *  Created on: May 2, 2013
 *      Author: Vaughn Friesen
 */

#include <vector>
#include <string>
#include <QThread>
#include <QMutex>
#include "muparser/muparser.h"

#ifndef PATHDATA_HPP_
#define PATHDATA_HPP_

class DisplayWorker;

#define NUM_PATHS 3

struct Path {
	Path();

	Path &operator=(const Path &other);

	bool setEqn(std::string eqn, bool funcOfY);
	void evaluate(DisplayWorker *displayWorker, std::size_t count);
	const std::vector<double> &getParams(int *init);
	void draw(DisplayWorker *displayWorker);

	mu::Parser parser;
	std::string eqn;
	std::size_t count;
	double start; // Start and end could be either x or y values
	double end;
	bool funcOfY;
	double input;

	mutable QMutex mutex;
	std::vector<double> params; // Params could be either functions of x or functions of y
};

struct PathData {
	PathData();
	PathData(const PathData &copy);

	Path paths[NUM_PATHS];
	const int numPaths;
};

#endif /* PATHDATA_HPP_ */
