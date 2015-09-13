/*
 * displayworker.hpp
 *
 *  Created on: Apr 29, 2013
 *      Author: Vaughn Friesen
 */

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QString>
#include <vector>
#include <screen/screen.h>
#include "graphlines.hpp"
#include "workerbase.hpp"
#include "pathdata.hpp"

#ifndef DISPLAYWORKER_HPP_
#define DISPLAYWORKER_HPP_

#define MIN_ZOOMX 0.01f
#define MIN_ZOOMY 0.01f
#define DEFAULT_ZOOMX 0.125f
#define DEFAULT_ZOOMY 0.125f
#define MAX_ZOOMX 10000.0f
#define MAX_ZOOMY 10000.0f
#define MIN_PANX -10000.0f
#define MIN_PANY -10000.0f
#define DEFAULT_PANX 0.0f
#define DEFAULT_PANY 0.0f
#define MAX_PANX 10000.0f
#define MAX_PANY 10000.0f

class DisplayWorker : public WorkerBase {
	Q_OBJECT
	Q_PROPERTY(bool displayBusy READ isDisplayBusy NOTIFY displayBusyChanged)

public:
	DisplayWorker();
	virtual ~DisplayWorker();

	void setPathDataDisplayable(PathData *pathData);

	void setDisplayBusy(bool busy);
	bool isDisplayBusy();

	Q_INVOKABLE bool createGraphWindow(const QString &group, const QString &id, int width, int height);

	int getWidth() { return size[0]; }
	int getHeight() { return size[1]; }
	Q_INVOKABLE inline void setSizeChanged() {
		sizeUpdate = true;
	}

	void lineFromTo(unsigned char *buf, unsigned char *col, int x0, int y0, int x1, int y1);

	inline double screenXToReal(int screen) {
	  return ((double)screen - bx) / ax;
	}

	inline float screenYToReal(int screen) {
	  return ((double)screen - by) / ay;
	}

	inline int realXToScreen(double screen) {
	  return (int)(screen * ax + bx);
	}

	inline int realYToScreen(double screen) {
	  return (int)(screen * ay + by);
	}

signals:
    void displayBusyChanged(bool displayBusy);

protected:
    PathData *nextDisplayable;

    // These are thread safe because of the mutex
    mutable QMutex mutex;
    PathData *paths;
	bool nextUpdated;

	volatile bool displayBusy;

	// Screen buffers and windows
	volatile bool screenCreated, needsUpdate, sizeUpdate;
	screen_context_t screenContext;
	screen_window_t screenWindow;
	screen_buffer_t screenBuffer[1];
	screen_buffer_t screenPixelBuffer;
	screen_pixmap_t screenPix;
	int size[2];
	int rect[4];
	int stride;

	// Range and domain
	double ax, bx, ay, by;
	double rangeLeft, rangeRight, domainTop, domainBottom;

	// Zoom and pan
	float zoomX, zoomY, panX, panY;
	float startZoomX, startZoomY, startPanX, startPanY, startRatioX, startRatioY;

	GraphLines *lines;

	void beginProcess();
	bool tryProcess();
	void endProcess();

	void draw();
	void setZoomPan(float zoomX, float zoomY, float panX, float panY);
	void setParams(double rangeLeft, double rangeRight, double domainTop, double domainBottom);
	void lineFromToCareful(unsigned char *buf, unsigned char *col, int x0, int y0, int x1, int y1);
};

#endif /* DISPLAYWORKER_HPP_ */
