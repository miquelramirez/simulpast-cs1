
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
class GujaratDemographics;

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

	virtual bool checkEmigration();

protected:
	int _availableTime; // MpiAttribute
	int _spentTime; // MpiAttribute
	int _collectedResources; // MpiAttribute

	// age of the agent in num steps (years*3)
	int _age; // MpiAttribute

	// allowed range for social interaction
	int _socialRange; // MpiAttribute

	int _homeMobilityRange; // MpiAttribute

	float _massToCaloriesRate;
	float _surplusSpoilageFactor;

	float _walkingSpeedHour;
	float _forageTimeCost;
	float _availableForageTime;
	float _emigrationProbability;
	float _reproductionProbability;

	std::list<Action*> _actions;

	AgentController*	_controller;
	GujaratDemographics*	_demographicsModel;

	bool _starved;
	
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
	int	getPopulationSize() const;

	// MRJ: Kills 1 out of every 10 people in the agent (on average)
	void	decimatePopulation();
	// MRJ: Checks if agent member dies with chance%, with age in [min,max]
	void	checkDeath( int minAge, int maxAge, int chance );
	void	checkDeathByAging( int minAge );
	// if male or female died, reproduction is impossible
	bool	canReproduce() const;
	void	addNewChild();

	int	getOnHandResources() const { return _collectedResources; }
	int	computeConsumedResources( int timeSteps ) const;
	double	computeMaxForagingDistance( ) const;
	int	computeEffectiveBiomassForaged( int nominal ) const;
	int	convertBiomassToCalories( int biomass ) const;
	bool	starvationLastTimeStep() const { return _starved; }

	void	setSurplusSpoilageFactor( float v ) { _surplusSpoilageFactor = v; }
	float	getSurplusSpoilageFactor() const { return _surplusSpoilageFactor; }

	void	setWalkingSpeedHour( float v ) { _walkingSpeedHour = v; }
	float	getWalkingSpeedHour() const { return _walkingSpeedHour; }

	void	setForageTimeCost( float v ) { _forageTimeCost = v; }
	float	getForageTimeCost() const { return _forageTimeCost; }

	void	setAvailableForageTime( float v ) { _availableForageTime = v; }
	float	getAvailableForageTime() const { return _availableForageTime; }

	void	setEmigrationProbability( float v ) { _emigrationProbability = v; }
	float	getEmigrationProbability() const { return _emigrationProbability; }
	void	setReproductionProbability( float v ) { _reproductionProbability = v; }
	float	getReproductionProbability() const { return _reproductionProbability; }

	double	getTimeSpentForagingTile() const;

	void			setController( AgentController* controller ); 
	AgentController* 	activeController() { return _controller; }
	void			setDemographicsModel( GujaratDemographics* model ) { _demographicsModel = model; }
	std::ostream&		log() { return *_log; }	
};

} // namespace Gujarat

#endif // __GujaratAgent_hxx__

