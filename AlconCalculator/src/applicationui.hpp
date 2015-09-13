// Application with UI adaptability support template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include "mathengine/mathengine.hpp"
#include "mathengine/displayworker.hpp"
#include "mathengine/mathworker.hpp"

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QThread
{
    Q_OBJECT

public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI();

public slots:
	void aboutToQuit();

private:
    MathEngine *mathEngine;
    DisplayWorker *displayWorker;
    MathWorker *mathWorker;
};


#endif /* ApplicationUI_HPP_ */
