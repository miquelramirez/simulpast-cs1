
#include "ResultsAnalysis.hxx"

#include "Analysis.hxx"
#include "SimulationRecord.hxx"

#include <iostream>
#include <fstream>

namespace Analysis
{

ResultsAnalysis::ResultsAnalysis( Engine::SimulationRecord & simRecord, const std::string & outputFile, const std::string & agentType, const std::string & separator ) : _simRecord(simRecord), _outputFile(outputFile), _agentType(agentType), _separator(separator)
{
}

ResultsAnalysis::~ResultsAnalysis()
{
	AnalysisList::iterator it =_analysisList.begin();
	while(it!=_analysisList.end())
	{
		Analysis * analysis = *it;
		it = _analysisList.erase(it);
		delete analysis;
	}
}

void ResultsAnalysis::apply() const
{	
	// pre process
	for(AnalysisList::const_iterator itL=_analysisList.begin(); itL!=_analysisList.end(); itL++)
	{
		(*itL)->preProcess();
	}
	
	std::ofstream file;
	file.open(_outputFile.c_str());
  
	std::stringstream header;
	header << "timeStep" << _separator;
	for(AnalysisList::const_iterator it=_analysisList.begin(); it!=_analysisList.end(); it++)
	{
		header << (*it)->getName() << ";";
	}
	file << header.str() << std::endl;;
	
	// all agents
	if(_agentType.compare("all")==0)
	{
		for(Engine::SimulationRecord::AgentTypesMap::const_iterator it=_simRecord.beginTypes(); it!=_simRecord.endTypes(); it++)
		{
			const Engine::SimulationRecord::AgentRecordsMap & agentRecords = it->second;
			for(Engine::SimulationRecord::AgentRecordsMap::const_iterator itA=agentRecords.begin(); itA!=agentRecords.end(); itA++)
			{
				for(AnalysisList::const_iterator itL=_analysisList.begin(); itL!=_analysisList.end(); itL++)
				{
					(*itL)->computeAgent(*(itA->second));
				}
			}
		}
	}
	else
	{
		for(Engine::SimulationRecord::AgentRecordsMap::const_iterator it=_simRecord.beginAgents(_agentType); it!=_simRecord.endAgents(_agentType); it++)
		{
			for(AnalysisList::const_iterator itL=_analysisList.begin(); itL!=_analysisList.end(); itL++)
			{
				(*itL)->computeAgent(*it->second);
			}
		}
	}	
	
	// post process
	for(AnalysisList::const_iterator itL=_analysisList.begin(); itL!=_analysisList.end(); itL++)
	{
		(*itL)->postProcess();
	}

	for(int i=0; i<_simRecord.getNumSteps(); i++)
	{
		std::stringstream newLine;
		newLine << i << _separator;
		for(AnalysisList::const_iterator itL=_analysisList.begin(); itL!=_analysisList.end(); itL++)
		{
			newLine << (*itL)->getResult(i) << ";";
		}
		file << newLine.str() << std::endl;
	}
	file.close();
}

void ResultsAnalysis::addAnalysis( Analysis * analysis )
{
	analysis->setNumTimeSteps(_simRecord.getNumSteps());
	_analysisList.push_back(analysis);
}

} // namespace Analysis

