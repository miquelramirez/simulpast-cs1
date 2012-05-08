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

	virtual	Action*	selectAction();
	
	Action*         shouldDoNothing(  );
	Action*         shouldForage(  );
	Action*         shouldMoveHome(  );
	
	Sector*         getMaxBiomassSector(  );
	unsigned        getDoNothingDaysCovered() const { return _DoNothingDaysCovered; } 

private:
	HunterGatherer*		_agentConcrete;
	unsigned                _DoNothingDaysCovered;
};

}

#endif // HunterGathererDecisionTreeController.hxx
