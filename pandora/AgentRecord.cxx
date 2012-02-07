
#include "AgentRecord.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <sstream>

namespace Engine
{

AgentRecord::AgentRecord( const std::string & id, int numSteps ) : _id(id), _numSteps(numSteps)
{
}

AgentRecord::~AgentRecord()
{
}

void AgentRecord::addState( int numStep, const std::string & key, int value )
{
	StatesMap::iterator it = _states.find(key);
	// the key already exists
	if(it==_states.end())
	{
		//_states.insert( std::pair< std::string, ValuesVector >(key, std::vector<int> ));		
		std::vector<int> values;
		values.resize(_numSteps);
		_states.insert( make_pair( key, values ));
		it = _states.find(key);
		/*
		ValuesVector & vector = it->second;
		vector.resize(_numSteps);
		*/
	}
	ValuesVector & vector = it->second;
	vector[numStep] = value;
}

int AgentRecord::getState( int numStep, const std::string & key )
{
	StatesMap::iterator it = _states.find(key);
	// the key already exists
	if(it==_states.end())
	{
		std::stringstream oss;
		oss << "AgentRecord::getState - searching for unknown key: " << key << " in agent record";
		throw Engine::Exception(oss.str());
	}
	ValuesVector & vector = it->second;
	return vector[numStep];
}

std::string AgentRecord::getCompleteState( int numStep )
{
	std::stringstream completeState;
	completeState << "id: " << _id << std::endl;
	completeState << "Agent values:";
	for(StatesMap::iterator it=_states.begin(); it!=_states.end(); it++)
	{
		completeState << std::endl << it->first << ": " << it->second[numStep];
	}
	return completeState.str();
}

AgentRecord::StatesMap::iterator AgentRecord::beginStates()
{
	return _states.begin();
}

AgentRecord::StatesMap::iterator AgentRecord::endStates()
{
	return _states.end();
}

} // namespace Engine

