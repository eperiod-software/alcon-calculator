/*
 * graphpath.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: Vaughn Friesen
 */

#if 0

#include <limits>
#include "graphpath.hpp"
#include "graphengine.hpp"

#define DEBUG_CLASS "GraphPath"
#include "debug.hpp"

using namespace std;
using namespace mu;

GraphPath::GraphPath(GraphEngine *ge, string eqn) :
		eqn(eqn), ge(ge), ys(NULL), errs(NULL), infs(NULL), width(0), height(0), funcOfY(false) {
	needEvaluate = true;
	input = 0;

	// Check if there's an "x=" or "y="
	bool funcOfX = false;
	size_t equals = eqn.find('=');
	if (equals != std::string::npos) {
		if (eqn.substr(0, equals) == "x")
			funcOfY = true;
		else if (eqn.substr(0, equals) == "y")
			funcOfX = true;
		else {
			// Error - not x or y
			this->eqn = "";
			return;
		}

		this->eqn = eqn = eqn.substr(equals + 1);
	}

	parser.DefineConst("pi", (double)M_PI);
	parser.DefineConst("e", (double)M_E);

	parser.SetExpr(eqn);

	if (!funcOfX) {
		parser.DefineVar("y", &input);

		try
		{
			parser.Eval();
			funcOfY = true;
			return;
		}
		catch (Parser::exception_type &e)
		{
			parser.RemoveVar("y");
		}
	}
	if (!funcOfY) {
		parser.DefineVar("x", &input);

		try
		{
			parser.Eval();
			funcOfY = false;
			return;
		}
		catch (Parser::exception_type &e)
		{
			parser.RemoveVar("x");
		}
	}

	this->eqn = "";
}

GraphPath::~GraphPath() {
	if (ys)
		delete []ys;
	if (errs)
		delete []errs;
	if (infs)
		delete []infs;
}

bool GraphPath::draw(unsigned char *buf, unsigned char *col) {
	if (width != ge->getWidth() || height != ge->getHeight()) {
		width = ge->getWidth();
		height = ge->getHeight();
		if (ys)
			delete []ys;
		if (errs)
			delete []errs;
		if (infs)
			delete []infs;

		ys = new double[width];
		errs = new bool[width];
		infs = new bool[width];

		needEvaluate = true;
	}

	if (needEvaluate) {
		needEvaluate = false;
		if (!evaluate())
			return false;
	}

	int prevX = 0;
	int prevY = ge->realYToScreen(ys[0]);

	unsigned char asympCol[] = { 0xff, 0, 0xff, 0xff };

	for (int x = 1; x < width; x++) {
		int y = ge->realYToScreen(ys[x]);

		if (infs[x]) {
			// Vertical asymptote
			ge->lineFromTo(buf, asympCol, x, 0, x, height - 1);
		}
		else if (!infs[x - 1] && !errs[x - 1] && !errs[x]) {
			// TODO do something nice if something has an error. For now, I just don't draw the line
			ge->lineFromTo(buf, col, prevX, prevY, x, y);
		}

		prevX = x;
		prevY = y;
	}

	return true;
}

void GraphPath::setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom) {
	(void)rangeLeft;
	(void)rangeRight;
	(void)domainTop;
	(void)domainBottom;

	needEvaluate = true;
}

bool GraphPath::evaluate() {
	memset(errs, 0, sizeof(bool) * width); // Set all errors and infinity to false
	memset(infs, 0, sizeof(bool) * width);

	for (int x = 0; x < width; x++) {
		input = ge->screenXToReal(x);

		try
		{
			ys[x] = parser.Eval();

			if (abs(ys[x]) == std::numeric_limits<double>::infinity())
				infs[x] = true;
		}
		catch (Parser::exception_type &e)
		{
			errs[x] = true;
		}
	}

	return true;
}

#endif
