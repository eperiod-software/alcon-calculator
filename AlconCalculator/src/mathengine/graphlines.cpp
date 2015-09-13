/*
 * graphlines.cpp
 *
 *  Created on: Apr 23, 2013
 *      Author: Vaughn Friesen
 */

#include <math.h>
#include "graphlines.hpp"
#include "displayworker.hpp"

using namespace std;

GraphLines::GraphLines(DisplayWorker *dw) : dw(dw) {

}

GraphLines::~GraphLines() {

}

void GraphLines::setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom) {
	this->rangeLeft = rangeLeft;
	this->rangeRight = rangeRight;
	this->domainTop = domainTop;
	this->domainBottom = domainBottom;
}

bool GraphLines::draw(unsigned char *buf) {
	// First find the x and y axes
	double range = rangeRight - rangeLeft;
	double domain = domainTop - domainBottom;

	int w = dw->getWidth();
	int h = dw->getHeight();

	int originX = dw->realXToScreen(0);
	int originY = dw->realYToScreen(0);

	unsigned char black[] = { 0, 0, 0, 0xff };

	if (originX >= 0 && originX < w)
		dw->lineFromTo(buf, black, originX, 0, originX, h - 1);
	if (originY >= 0 && originY < h)
		dw->lineFromTo(buf, black, 0, originY, w - 1, originY);

	// Find which lines should be drawn
	static const int maxfreqs = 3, maxPix = 500, minPix = 50;
	double freqsX[maxfreqs];
	double freqsY[maxfreqs];
	int pixBetweenX[maxfreqs];
	int pixBetweenY[maxfreqs];
	int numX = 0;
	int numY = 0;

	memset(pixBetweenX, 0, sizeof(int) * maxfreqs);
	memset(pixBetweenY, 0, sizeof(int) * maxfreqs);

	for (int x = -15; x < 16; x++) {
		double freq = pow(2, x);

		// See if there are the right amount of pixels between each
		if (numX < maxfreqs) {
			int pix = (int)((double)w / (range / freq));

			if (pix > minPix && pix < maxPix) {
				pixBetweenX[numX] = pix;
				freqsX[numX] = freq;
				numX++;
			}
		}

		if (numY < maxfreqs) {
			int pix = (int)((double)h / (domain / freq));

			if (pix > minPix && pix < maxPix) {
				pixBetweenY[numY] = pix;
				freqsY[numY] = freq;
				numY++;
			}
		}

		if (numX == maxfreqs && numY == maxfreqs)
			break;
	}

	// Draw each line
	bool highestLevel;
	unsigned char gray = 0;
	int lowestGray = (w < h ? w : h), cur, tick, screen;
	double real;
	for (int x = 0; x < maxfreqs; x++) {
		// Start from the origin and draw every other one if there is another level, since the next
		// higher level will get the rest
		highestLevel = (x == maxfreqs - 1 || pixBetweenX[x + 1] == 0);

		// First start at the origin and go left until we're past the edge
		cur = 1;
		while (true) {
			real = -freqsX[x] * cur;
			screen = dw->realXToScreen(real);
			gray = 255 - pixBetweenX[x] * 255 / lowestGray;
			tick = x * 5 + 5;

			unsigned char grayCol[] = { gray, gray, gray, 0xff };

			if (screen <= 0)
				break;

			if (screen < w - 1) {
				dw->lineFromTo(buf, grayCol, screen, 0, screen, h - 1);

				// Draw a tick at each edge
				dw->lineFromTo(buf, black, screen, 0, screen, tick);
				dw->lineFromTo(buf, black, screen, h - 1, screen, h - 1 - tick);
			}

			// Draw a tick at the origin
			if (originY >= 0 && originY < h)
				dw->lineFromTo(buf, black, screen, originY - tick, screen, originY + tick);

			if (highestLevel)
				cur++;
			else
				cur += 2;
		}

		// Then go to the right
		cur = 1;
		while (true) {
			real = freqsX[x] * cur;
			screen = dw->realXToScreen(real);
			gray = 255 - pixBetweenX[x] * 255 / lowestGray;
			tick = x * 5 + 5;

			unsigned char grayCol[] = { gray, gray, gray, 0xff };

			if (screen >= w - 1)
				break;

			if (screen > 0) {
				dw->lineFromTo(buf, grayCol, screen, 0, screen, h - 1);

				// Draw a tick at each edge
				dw->lineFromTo(buf, black, screen, 0, screen, tick);
				dw->lineFromTo(buf, black, screen, h - 1, screen, h - 1 - tick);
			}

			// Draw a tick at the origin
			if (originY >= 0 && originY < h)
				dw->lineFromTo(buf, black, screen, originY - tick, screen, originY + tick);

			if (highestLevel)
				cur++;
			else
				cur += 2;
		}
	}

	for (int x = 0; x < maxfreqs; x++) {
		highestLevel = (x == maxfreqs - 1 || pixBetweenY[x + 1] == 0);

		// First start at the origin and go up until we're past the top
		cur = 1;
		while (true) {
			real = freqsY[x] * cur;
			screen = dw->realYToScreen(real);
			gray = 255 - pixBetweenY[x] * 255 / lowestGray;
			tick = x * 5 + 5;

			unsigned char grayCol[] = { gray, gray, gray, 0xff };

			if (screen <= 0)
				break;

			if (screen < h - 1) {
				dw->lineFromTo(buf, grayCol, 0, screen, w - 1, screen);

				// Draw a tick at each edge
				dw->lineFromTo(buf, black, 0, screen, tick, screen);
				dw->lineFromTo(buf, black, w - 1, screen, w - tick - 1, screen);
			}

			// Draw a tick at the origin
			if (originX >= 0 && originX < w)
				dw->lineFromTo(buf, black, originX - tick, screen, originX + tick, screen);

			if (highestLevel)
				cur++;
			else
				cur += 2;
		}

		// Then go to the bottom
		cur = 1;
		while (true) {
			real = -freqsY[x] * cur;
			screen = dw->realYToScreen(real);
			gray = 255 - pixBetweenY[x] * 255 / lowestGray;
			tick = x * 5 + 5;

			unsigned char grayCol[] = { gray, gray, gray, 0xff };

			if (screen >= h - 1)
				break;

			if (screen > 0) {
				dw->lineFromTo(buf, grayCol, 0, screen, w - 1, screen);

				// Draw a tick at each edge
				dw->lineFromTo(buf, black, 0, screen, tick, screen);
				dw->lineFromTo(buf, black, w - 1, screen, w - tick - 1, screen);
			}

			// Draw a tick at the origin
			if (originX >= 0 && originX < w)
				dw->lineFromTo(buf, black, originX - tick, screen, originX + tick, screen);

			if (highestLevel)
				cur++;
			else
				cur += 2;
		}
	}

	return true;
}
