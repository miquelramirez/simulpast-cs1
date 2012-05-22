
#ifndef __Mean_hxx__
#define __Mean_hxx__

#include "Analysis.hxx"

namespace Engine
{
	class AgentRecord;
}

namespace Analysis
{

class Mean : public Analysis
{
	std::string _attributeName;
	std::vector<int> _numAgents;
public:
	Mean( const std::string attributeName );
	virtual ~Mean();
	void computeAgent( const Engine::AgentRecord & agentRecord );

	void preProcess();
	void postProcess();
};

} // namespace Analysis

#endif // __Mean_hxx__

