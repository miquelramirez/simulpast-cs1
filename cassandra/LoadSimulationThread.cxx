
#include "LoadSimulationThread.hxx"
#include "ProjectConfiguration.hxx"

namespace GUI
{

LoadSimulationThread::LoadSimulationThread()
{
}

LoadSimulationThread::~LoadSimulationThread()
{
}

void LoadSimulationThread::run()
{
	emit simulationLoaded(ProjectConfiguration::instance()->loadSimulation());
}
	
} // namespace GUI

