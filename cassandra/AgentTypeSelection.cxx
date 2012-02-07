
#include "AgentTypeSelection.hxx"
#include "SimulationRecord.hxx"

#include <algorithm>

#include <iostream>

namespace GUI
{

AgentTypeSelection::AgentTypeSelection(QWidget * parent ) : QListWidget(parent), _simulationRecord(0)
{	  
	setMouseTracking(true);
}

AgentTypeSelection::~AgentTypeSelection()
{
}

void AgentTypeSelection::setSimulationRecord( Engine::SimulationRecord * simulationRecord )
{
	_simulationRecord = simulationRecord;
	clear();
	if(!simulationRecord)
	{
		update();
		return;
	}
	for(Engine::SimulationRecord::AgentTypesMap::iterator it=_simulationRecord->beginTypes(); it!=_simulationRecord->endTypes(); it++)
	{
		addItem(it->first.c_str());
	}    
	update();
}

} // namespace GUI

