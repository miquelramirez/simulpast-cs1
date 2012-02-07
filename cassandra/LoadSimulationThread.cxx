
#include "LoadSimulationThread.hxx"

#include "ProjectConfiguration.hxx"

#include <iostream>

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

