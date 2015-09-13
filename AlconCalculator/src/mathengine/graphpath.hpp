/*
 * graphpath.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Vaughn Friesen
 */

#if 0

#include <string>
#include <QMap>
#include <muparser/muparser.h>

#ifndef GRAPHPATH_HPP_
#define GRAPHPATH_HPP_

class GraphEngine;

class GraphPath {
public:
	GraphPath(GraphEngine *ge, std::string eqn);
	virtual ~GraphPath();

	bool draw(unsigned char *buf, unsigned char *col);
	void setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom);

	inline bool isValid() { return (eqn.length() > 0); }

private:
	std::string eqn;

	mu::Parser parser;

	GraphEngine *ge;

	QMap<QString, double> variables;

	double *ys;
	bool *errs;
	bool *infs;
	int width, height; // Width/height of the window (in pixels)
	double input;
	bool funcOfY;

	bool needEvaluate;

	bool evaluate();
};

#endif /* GRAPHPATH_H_ */

#endif
