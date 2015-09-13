/*
 * graphlines.hpp
 *
 *  Created on: Apr 23, 2013
 *      Author: Vaughn Friesen
 */

//#include <ft2build.h>
//#include FT_FREETYPE_H

#ifndef GRAPHLINES_HPP_
#define GRAPHLINES_HPP_

class DisplayWorker;

class GraphLines {
public:
	GraphLines(DisplayWorker *dw);
	virtual ~GraphLines();

	void setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom);

	bool draw(unsigned char *buf);

private:
	DisplayWorker *dw;
	double rangeLeft, rangeRight, domainTop, domainBottom;
};

#endif /* GRAPHLINES_HPP_ */
