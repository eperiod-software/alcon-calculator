/*
 * displayworker.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: Vaughn Friesen
 */

#include "displayworker.hpp"

#define DEBUG_CLASS "DisplayWorker"
#include "debug.hpp"

using namespace std;

DisplayWorker::DisplayWorker() : WorkerBase(), nextDisplayable(NULL), paths(NULL),
		zoomX(0.0f), zoomY(0.0f), panX(0.0f), panY(0.0f), lines(NULL) {
	nextUpdated = false;
	displayBusy = false;
	screenCreated = false;
	needsUpdate = false;
	sizeUpdate = false;
}

DisplayWorker::~DisplayWorker() { }

void DisplayWorker::setPathDataDisplayable(PathData *paths) {
	QMutexLocker locker(&mutex);

	if (nextDisplayable)
		delete nextDisplayable; // Replace with more current displayable

	nextDisplayable = paths;
	nextUpdated = true;
}

void DisplayWorker::setDisplayBusy(bool displayBusy) {
	this->displayBusy = displayBusy;
	emit displayBusyChanged(displayBusy);
}

bool DisplayWorker::isDisplayBusy() {
	return displayBusy;
}

bool DisplayWorker::createGraphWindow(const QString &group, const QString &id, int width, int height) {
	QMutexLocker locker(&mutex);

	QByteArray groupArr = group.toAscii();
	QByteArray idArr = id.toAscii();

	if (width <= 0 || height <= 0)
		return false;

	DEBUG_PRINT2("Creating graph window. w: %d, h: %d", width, height);

	// Window source rectangle
	rect[0] = 0;
	rect[1] = 0;
	rect[2] = width;
	rect[3] = height;

	// We must create a context before you create a window.
	if (screen_create_context(&screenContext, SCREEN_APPLICATION_CONTEXT) != 0) {
		return false;
	}

	// Create a child window of the current window group, join the window group and set
	// a window ID.
	if (screen_create_window_type(&screenWindow, screenContext, SCREEN_CHILD_WINDOW) != 0) {
		return false;
	}
	if (screen_join_window_group(screenWindow, groupArr.constData()) != 0) {
		return false;
	}
	if (screen_set_window_property_cv(screenWindow, SCREEN_PROPERTY_ID_STRING, idArr.length(),
			idArr.constData()) != 0) {
		return false;
	}

	// In this application, we will render to a pixmap buffer and then blit that to
	// the window, and then finally we set the usage to native.
	// (default is read and write but we do not need that here)
	int usage = SCREEN_USAGE_NATIVE;
	if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_USAGE, &usage) != 0) {
		return false;
	}

	// The window size is specified in QML so we need to set up the buffer size to
	// correspond to that, the default size would be the full screen.
	if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, rect + 2) != 0) {
		return false;
	}
	if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_SOURCE_SIZE, rect + 2) != 0) {
		return false;
	}

	// Use negative Z order so that the window appears under the main window.
	// (required by the ForeignWindow functionality)
	int z = -5;
	if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_ZORDER, &z) != 0) {
		return false;
	}

	// Set the window position on screen.
	if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_POSITION, rect) != 0) {
		return false;
	}

	// Create the window buffers, in this application we will only use one buffer.
	if (screen_create_window_buffers(screenWindow, 1) != 0) {
		return false;
	}

	// Create a pixmap to be used as off-screen rendering target. We can use this pixmap
	// to render other pixmaps, allowing us to have full control of exactly which pixels
	// we choose to push to the screen.
	if (screen_create_pixmap(&screenPix, screenContext) != 0) {
		return false;
	}

	// A combination of write and native usage is necessary to blit the pixmap to screen.
	usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
	if (screen_set_pixmap_property_iv(screenPix, SCREEN_PROPERTY_USAGE, &usage) != 0) {
		return false;
	}

	// Set the width and height of the buffer to correspond to the one we specified in the QML document.
	size[0] = width;
	size[1] = height;
	if (screen_set_pixmap_property_iv(screenPix, SCREEN_PROPERTY_BUFFER_SIZE, size) != 0) {
		return false;
	}

	// Create the pixmap buffer and get a reference to it for rendering in the doNoise function.
	if (screen_create_pixmap_buffer(screenPix) != 0) {
		return false;
	}
	if (screen_get_pixmap_property_pv(screenPix, SCREEN_PROPERTY_RENDER_BUFFERS,
			(void **) &screenPixelBuffer) != 0) {
		return false;
	}

	// Obtain the stride (the number of bytes between pixels on different rows), for use
	// later on when we perform the rendering to the pixmap buffer.
	if (screen_get_buffer_property_iv(screenPixelBuffer, SCREEN_PROPERTY_STRIDE, &stride) != 0) {
		return false;
	}

	setZoomPan(DEFAULT_ZOOMX, DEFAULT_ZOOMY * (double)width / (double)height, DEFAULT_PANX, DEFAULT_PANY);

	screenCreated = true;
	sizeUpdate = false;

	DEBUG_PRINT("Creating graph window successful");

	return true;
}

void DisplayWorker::beginProcess() {
	DEBUG_PRINT("Display worker thread started");

	lines = new GraphLines(this);
}

bool DisplayWorker::tryProcess() {
	QMutexLocker locker(&mutex);

	if (!screenCreated)
		return false;

	if (!nextUpdated && !needsUpdate && !sizeUpdate)
		return false; // Return if 1) the path hasn't been updated and 2) the size hasn't been updated

	setDisplayBusy(true);

	// Replace the currently displayed path
	if (paths) {
		delete paths;
		paths = NULL;
	}

	if (nextDisplayable) {
		paths = nextDisplayable;
		delete nextDisplayable;
	}

	// Draw the graph here
	draw();

	setDisplayBusy(false);

	nextUpdated = false;

	return true;
}

void DisplayWorker::endProcess() {
	QMutexLocker locker(&mutex);

	if (paths)
		delete paths;
	if (nextDisplayable)
		delete nextDisplayable;
	if (lines)
		delete lines;

	DEBUG_PRINT("Display worker thread finished");
}


void DisplayWorker::draw() {
	unsigned char *ptr = NULL;

	// See if we need to update the buffer size
	if (sizeUpdate || size[0] == 0 || size[1] == 0) {
		float zx = zoomX, zy = zoomY;
		bool smartZoom = false;

		if (size[0] != 0 && size[1] != 0) {
			zx = zoomX / size[0];
			zy = zoomY / size[1];
			smartZoom = true;
		}

		if (screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, size) != 0) {
			usleep(passiveSleep);
			return; // Try again next frame
		}

		if (size[0] == 0 || size[1] == 0) {
			usleep(passiveSleep);
			return;
		}

		DEBUG_PRINT2("Window size changed. w: %d, h: %d", size[0], size[1]);

		rect[0] = rect[1] = 0;
		rect[2] = size[0];
		rect[3] = size[1];

		if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, rect + 2) != 0) {
			return;
		}
		if (screen_set_window_property_iv(screenWindow, SCREEN_PROPERTY_SOURCE_SIZE, rect + 2) != 0) {
			return;
		}
		if (screen_set_pixmap_property_iv(screenPix, SCREEN_PROPERTY_BUFFER_SIZE, size) != 0) {
			return;
		}
		if (screen_get_pixmap_property_pv(screenPix, SCREEN_PROPERTY_RENDER_BUFFERS,
				(void **) &screenPixelBuffer) != 0) {
			return;
		}

		// Obtain the stride (the number of bytes between pixels on different rows), for use
		// later on when we perform the rendering to the pixmap buffer.
		if (screen_get_buffer_property_iv(screenPixelBuffer, SCREEN_PROPERTY_STRIDE, &stride) != 0) {
			return;
		}

		if (smartZoom)
			setZoomPan(zx * size[0], zy * size[1], panX, panY);
		else
			setZoomPan(zx, zy, panX, panY);

		DEBUG_PRINT("Resizing graph window successful");

		sizeUpdate = false;
	}

	// Setup the background parameters to render a background color over the entire child window
	int backParameters[] = {
			SCREEN_BLIT_SOURCE_WIDTH, size[0],
			SCREEN_BLIT_SOURCE_HEIGHT, size[1],
			SCREEN_BLIT_END
	};

	// Get the pixmap buffer and fill it with the background
	screen_get_buffer_property_pv(screenPixelBuffer, SCREEN_PROPERTY_POINTER, (void **) &ptr);
	unsigned char *origPtr = ptr;

	int thisW = size[0];
	int thisH = size[1];

	// Clear with white
	for (int i = 0; i < thisH; i++, ptr += stride) {
		memset(ptr, 0xff, thisW * 4);
	}

	ptr = origPtr;

	lines->draw(ptr);

	if (paths) {
		for (int x = 0; x < paths->numPaths; x++) {
			paths->paths[x].draw(this);
		}
	}

	/*unsigned char col1[] = { 0, 0, 0xff, 0xff };
	unsigned char col2[] = { 0xff, 0, 0, 0xff };
	unsigned char col3[] = { 0, 0xff, 0, 0xff };

	if (path1)
		path1->draw(ptr, col1);
	if (path2)
		path2->draw(ptr, col2);
	if (path3)
		path3->draw(ptr, col3);*/

	// Get the window buffer, blit the pixels to the buffer and post the window update.
	screen_get_window_property_pv(screenWindow, SCREEN_PROPERTY_RENDER_BUFFERS,
			(void **) screenBuffer);
	screen_blit(screenContext, screenBuffer[0], screenPixelBuffer, backParameters);
	screen_post_window(screenWindow, screenBuffer[0], 1, rect, 0);

	needsUpdate = false;
}

void DisplayWorker::setZoomPan(float zoomX, float zoomY, float panX, float panY) {
	this->zoomX = zoomX;
	this->zoomY = zoomY;
	this->panX = panX;
	this->panY = panY;

	double left = -1.0, right = 1.0;
	double top = 1.0, bottom = -1.0;

	left += panX / zoomX;
	right += panX / zoomX;
	top += panY / zoomY;
	bottom += panY / zoomY;

	left -= 1.0f / zoomX;
	right += 1.0f / zoomX;
	top += 1.0f / zoomY;
	bottom -= 1.0f / zoomY;

	/*left /= zoomX;
	right /= zoomX;
	top /= zoomY;
	bottom /= zoomY;

	// Set pan
	left += panX / zoomX;
	right += panX / zoomX;
	top += panY / zoomY;
	bottom += panY / zoomY;*/

	setParams(left, right, top, bottom);
}

void DisplayWorker::setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom) {
	this->rangeLeft = rangeLeft;
	this->rangeRight = rangeRight;
	this->domainTop = domainTop;
	this->domainBottom = domainBottom;

	float range = rangeRight - rangeLeft;
	ax = size[0] / range;
	bx = -rangeLeft * size[0] / range;

	float domain = domainBottom - domainTop;
	ay = size[1] / domain;
	by = -domainTop * size[1] / domain;

	needsUpdate = true;

	/*if (path1)
		path1->setParams(rangeLeft, rangeRight, domainTop, domainBottom);
	if (path2)
		path2->setParams(rangeLeft, rangeRight, domainTop, domainBottom);
	if (path3)
		path3->setParams(rangeLeft, rangeRight, domainTop, domainBottom);*/

	lines->setParams(rangeLeft, rangeRight, domainTop, domainBottom);
}

void DisplayWorker::lineFromTo(unsigned char *buf, unsigned char *col, int x0, int y0, int x1, int y1) {
	if (x0 < 0 || x0 >= size[0] || x1 < 0 || x1 >= size[0] ||
			y0 < 0 || y0 >= size[1] || y1 < 0 || y1 >= size[1]) {
		lineFromToCareful(buf, col, x0, y0, x1, y1);
		return;
	}

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (1) {
		int loc = x0 * 4 + y0 * stride;

		buf[loc] = col[0];
		buf[loc + 1] = col[1];
		buf[loc + 2] = col[2];
		buf[loc + 3] = col[3];

		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void DisplayWorker::lineFromToCareful(unsigned char *buf, unsigned char *col, int x0, int y0, int x1, int y1) {
	int w = size[0];
	int h = size[1];

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (1) {
		if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h) {
			int loc = x0 * 4 + y0 * stride;

			buf[loc] = col[0];
			buf[loc + 1] = col[1];
			buf[loc + 2] = col[2];
			buf[loc + 3] = col[3];
		}

		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}
