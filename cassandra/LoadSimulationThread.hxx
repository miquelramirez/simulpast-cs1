
#ifndef __LoadSimulationThread_hxx__
#define __LoadSimulationThread_hxx__

#include <QThread>

namespace GUI
{

class LoadSimulationThread : public QThread
{
	Q_OBJECT

public:
	LoadSimulationThread();
	virtual ~LoadSimulationThread();

	void run();
signals:
	void simulationLoaded(bool);
};

} // namespace GUI

#endif // __LoadSimulationThread_hxx__

