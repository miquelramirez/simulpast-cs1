
#include "AgentTraitSelection.hxx"
#include "SimulationRecord.hxx"
#include "AgentRecord.hxx"

#include <algorithm>

#include <iostream>
//#include <QLabel>
//#include <QListWidget>

namespace GUI
{

AgentTraitSelection::AgentTraitSelection(QWidget * parent ) : QListWidget(parent), _simulationRecord(0), _selectedType("unknown")
{	  
	setMouseTracking(true);
}

AgentTraitSelection::~AgentTraitSelection()
{
}

void AgentTraitSelection::setSimulationRecord( Engine::SimulationRecord * simulationRecord )
{
	_simulationRecord = simulationRecord;

	_selectedType = "unknown";
	_states.clear();
	clear();
	update();
}
	
void AgentTraitSelection::typeSelected( QListWidgetItem * item )
{
	_selectedType= item->text().toStdString();
	_states.clear();
	clear();
	for(Engine::SimulationRecord::AgentRecordsMap::iterator it=_simulationRecord->beginAgents(_selectedType); it!=_simulationRecord->endAgents(_selectedType); it++)
	{
		Engine::AgentRecord * agentRecord = it->second;
		for(Engine::AgentRecord::StatesMap::iterator itS=agentRecord->beginStates(); itS!=agentRecord->endStates(); itS++)
		{
			std::string stateName = itS->first;
			std::list<std::string>::iterator itL = std::find(_states.begin(), _states.end(), stateName);
			if(itL==_states.end())
			{
				_states.push_back(stateName);
				addItem(stateName.c_str());
			}
		}
	}    
}

} // namespace GUI

