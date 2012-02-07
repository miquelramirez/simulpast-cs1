
#ifndef __GujaratAgent_hxx__
#define __GujaratAgent_hxx__

#include "Agent.hxx"

#include <vector>
#include <list>

namespace Gujarat
{
class Action;

class GujaratAgent : public Engine::Agent
{
	// age of the agent in num steps (years*3)
//	int _age;
//	int _homeRange;

	// father = 0, mother = 1, children > 1
	std::vector<int> _populationAges;

	void updateAges();
	void checkReproduction();
	void checkMortality();
	void checkAgentRemoval();
	void checkMarriage();

	GujaratAgent * getMarriageCandidate();
	Engine::Point2D<int> getNearLocation();

	virtual void updateKnowledge() = 0;

	virtual void evaluateYearlyActions() = 0;
	virtual void evaluateSeasonalActions() = 0;
	virtual void evaluateIntraSeasonalActions() = 0;
	virtual GujaratAgent * createNewAgent() = 0;

	void executeActions();

	virtual void serializeAdditionalAttributes() = 0;

protected:
	const static int _availableTime = 1000;
	int _spentTime;
	int _collectedResources;

	int _age;
	int _homeRange;



	std::list<Action*> _actions;

public:
	GujaratAgent( const std::string & id );
	virtual ~GujaratAgent();

	void step();
	void serialize();

	virtual void moveHome();
};

} // namespace Gujarat

#endif // __GujaratAgent_hxx__

