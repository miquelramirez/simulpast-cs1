#ifndef __HUNTER_GATHERER_DECISION_TREE_CONTROLLER__
#define __HUNTER_GATHERER_DECISION_TREE_CONTROLLER__

#include "AgentController.hxx"

namespace Gujarat
{

class HunterGatherer;
class Sector;

class HunterGathererDecisionTreeController : public AgentController
{
public:
	HunterGathererDecisionTreeController( GujaratAgent* agent );
	virtual ~HunterGathererDecisionTreeController();

	virtual	MDPAction*	selectAction();
	
	MDPAction*         shouldDoNothing(  );
	MDPAction*         shouldForage(  );
	MDPAction*         shouldMoveHome(  );
	
	Sector*         getMaxBiomassSector(  );
	unsigned        getDoNothingDaysCovered() const { return _DoNothingDaysCovered; } 

private:
	HunterGatherer*		_agentConcrete;
	unsigned                _DoNothingDaysCovered;
};

}

#endif // HunterGathererDecisionTreeController.hxx
