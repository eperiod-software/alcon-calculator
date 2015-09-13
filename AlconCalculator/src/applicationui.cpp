// Application with UI adaptability support template
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>
#include <math.h>

#define DEBUG_CLASS "ApplicationUI"
#include "debug.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
{
	DEBUG_PRINT("Creating QML document");

    // Create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    DEBUG_PRINT("Creating math engine");

    mathEngine = new MathEngine();
    qml->setContextProperty("mathEngine", mathEngine);

    DEBUG_PRINT("Creating display worker thread");

    QThread* thread = new QThread;
    displayWorker = new DisplayWorker();
    displayWorker->moveToThread(thread);
    connect(displayWorker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), displayWorker, SLOT(process()));
    connect(displayWorker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(displayWorker, SIGNAL(finished()), displayWorker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

	qml->setContextProperty("displayWorker", displayWorker);

	DEBUG_PRINT("Creating math worker thread");

	thread = new QThread;
	mathWorker = new MathWorker(displayWorker);
	mathWorker->moveToThread(thread);
	connect(mathWorker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	connect(thread, SIGNAL(started()), mathWorker, SLOT(process()));
	connect(mathWorker, SIGNAL(finished()), thread, SLOT(quit()));
	connect(mathWorker, SIGNAL(finished()), mathWorker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();

	qml->setContextProperty("mathWorker", mathWorker);

    /*DEBUG_PRINT("Creating graph engine");

    graphEngine = new GraphEngine();*/

	DEBUG_PRINT("Creating QML root object");

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    DEBUG_PRINT("Setting scene root object");

    // Set created root object as a scene
    app->setScene(root);

    /*DEBUG_PRINT("Starting graph thread");

    start();

    // Start the graph engine thread
    graphEngine->start();*/
}

ApplicationUI::~ApplicationUI() {
}

void ApplicationUI::aboutToQuit() {
	DEBUG_PRINT("Cleaning up");

	delete mathEngine;
	delete displayWorker;
	delete mathWorker;
	mathEngine = NULL;
	displayWorker = NULL;
	mathWorker = NULL;
}
