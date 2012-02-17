
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

	// father = 0, mother = 1, children > 1
	std::vector<int> _populationAges;

	void updateAges();
	void checkReproduction();
	void checkMortality();
	void checkAgentRemoval();
	void checkMarriage();

	GujaratAgent * getMarriageCandidate();

	virtual void updateKnowledge() = 0;

	virtual void evaluateYearlyActions() = 0;
	virtual void evaluateSeasonalActions() = 0;
	virtual void evaluateIntraSeasonalActions() = 0;
	virtual GujaratAgent * createNewAgent() = 0;

	void executeActions();

	virtual void serializeAdditionalAttributes() = 0;

protected:
	int _availableTime;
	int _spentTime;
	int _collectedResources;
	

	// age of the agent in num steps (years*3)
	int _age;
	// allowed range for social interaction
	int _socialRange;

	int _homeMobilityRange;

	std::list<Action*> _actions;
	Engine::Point2D<int> getNearLocation( int range );

public:
	GujaratAgent( const std::string & id );
	virtual ~GujaratAgent();

	void step();
	void serialize();

	void	setSocialRange( int v ) { _socialRange = v; }
	int	getSocialRange() const { return _socialRange; }
	void	setAvailableTime( int daysPerSeason );
	void	setHomeMobilityRange( int v ) { _homeMobilityRange = v; }
	int	getHomeMobilityRange( ) { return _homeMobilityRange; } 
	
	void	initializePosition( Engine::Point2D<int> randomPos );
};

} // namespace Gujarat

#endif // __GujaratAgent_hxx__

