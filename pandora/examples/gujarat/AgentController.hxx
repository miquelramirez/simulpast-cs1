#ifndef __AGENT_CONTROLLER__
#define __AGENT_CONTROLLER__

#include "GujaratAgent.hxx"

namespace Gujarat
{

class Action;

class AgentController
{
public:
	
	AgentController( GujaratAgent* a );
	virtual ~AgentController();

	// MRJ: returned object ownership is caller's
	virtual Action* selectAction() = 0;

	GujaratAgent&		agentRef() { return *_agent; }
	const GujaratAgent& 	agentRef() const { return *_agent; }

private:

	GujaratAgent*		_agent;
};

}

#endif // AgentController.hxx
