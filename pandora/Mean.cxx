
#include "Mean.hxx"
#include "AgentRecord.hxx"

namespace Analysis
{

Mean::Mean( const std::string attributeName ) : Analysis("Mean "+attributeName), _attributeName(attributeName)
{
}

Mean::~Mean()
{
}

void Mean::preProcess()
{
	_numAgents.resize(_results.size());
}

void Mean::computeAgent( const Engine::AgentRecord & agentRecord )
{
	for(int i=0; i<_results.size(); i++)
	{
		int value = agentRecord.getState(i, _attributeName);
		_results[i] += value;
		_numAgents[i]++;
	}
}

void Mean::postProcess()
{
	for(int i=0; i<_results.size(); i++)
	{
		int numAgents = _numAgents.at(i);
		if(numAgents>0)
		{
			_results[i] /= numAgents;
		}
		else
		{
			_results[i] = 0.0f;
		}
	}
}

} // namespace Analysis

