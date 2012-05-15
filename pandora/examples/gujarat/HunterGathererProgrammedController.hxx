#ifndef __HUNTER_GATHERER_PROGRAMMED_CONTROLLER__
#define __HUNTER_GATHERER_PROGRAMMED_CONTROLLER__

#include "AgentController.hxx"

namespace Gujarat
{

class HunterGatherer;

class HunterGathererProgrammedController : public AgentController
{
public:
	HunterGathererProgrammedController( GujaratAgent* agent );
	virtual ~HunterGathererProgrammedController();

	virtual	MDPAction*	selectAction();

private:
	HunterGatherer*		_agentConcrete;
};

}

#endif // HunterGathererProgrammedController.hxx
