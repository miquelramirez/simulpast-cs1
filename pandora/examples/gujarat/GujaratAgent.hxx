
#ifndef __GujaratAgent_hxx__
#define __GujaratAgent_hxx__

#include "Agent.hxx"

#include <vector>
#include <list>
#include <fstream>

namespace Gujarat
{
class Action;
class AgentController;

class GujaratAgent : public Engine::Agent
{
	
	void updateAges();
	void checkReproduction();
	void checkStarvationMortality();
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
	int _availableTime; // MpiAttribute
	int _spentTime; // MpiAttribute
	int _collectedResources; // MpiAttribute

	// father = _populationAges[0], mother = _populationAges[1], children =_populationAges[> 1]
	std::vector<int> _populationAges;
	
	// age of the agent in num steps (years*3)
	int _age; // MpiAttribute

	// allowed range for social interaction
	int _socialRange; // MpiAttribute

	int _homeMobilityRange; // MpiAttribute

	float _massToCaloriesRate;

	std::list<Action*> _actions;

	AgentController*	_controller;

	bool _starvated;
	
	std::ofstream*		_log;
	
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
	int	getHomeMobilityRange( ) const { return _homeMobilityRange; } 
	
	void	setMassToCaloriesRate( float v ) { _massToCaloriesRate = v; }

	void	initializePosition( Engine::Point2D<int> randomPos );
	int	getNrAvailableAdults() const;
	int	getNrChildren() const;
	int	getOnHandResources() const { return _collectedResources; }
	
	virtual int neededResources( int timeSteps ) const;
	int	computeConsumedResources( int timeSteps ) const;
	double	computeMaxForagingDistance( ) const;
	int	computeEffectiveBiomassForaged( int nominal ) const;
	int	convertBiomassToCalories( int biomass ) const;
	bool	starvationLastTimeStep() const { return _starvated; }

	void			setController( AgentController* controller ); 
	AgentController* 	activeController() { return _controller; }
	std::ostream&		log() { return *_log; }	
};

} // namespace Gujarat

#endif // __GujaratAgent_hxx__

