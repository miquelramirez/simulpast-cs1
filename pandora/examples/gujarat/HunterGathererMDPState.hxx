#ifndef __HunterGathererMDPState_hxx__
#define __HunterGathererMDPState_hxx__

#include "Point2D.hxx"
#include "IncrementalRaster.hxx"
#include "HashTable.hxx"
#include "Action.hxx"
#include <engine/problem.h>

namespace Gujarat
{

class HunterGathererMDPProblem;

class HunterGathererMDPState
{
public:
	// Constructors, I don't want this to be ever invoked
	explicit HunterGathererMDPState();

	// The real one
	HunterGathererMDPState(Engine::Point2D<int> loc, int initialOnHand, const Engine::Raster& resourcesRaster);
	HunterGathererMDPState( const HunterGathererMDPState& s );
	const HunterGathererMDPState&	operator=(const HunterGathererMDPState& s );
	
	~HunterGathererMDPState();

	void		initializeSuccessor( HunterGathererMDPState& s ) const;

	unsigned	hash() const;
	bool		operator==( const HunterGathererMDPState& s ) const;
	bool		operator!=( const HunterGathererMDPState& s ) const;
	bool		operator<( const HunterGathererMDPState& s ) const;

	void		print( std::ostream& os ) const;

	void					increaseTimeIndex() { _timeIndex++; }
	unsigned				getTimeIndex() const { return _timeIndex; }
	int					getOnHandResources() const { return _onHandResources; }
	void					addResources( int amt ) { _onHandResources = _onHandResources + amt; }
	void					decreaseResources( int amt ) { _onHandResources = _onHandResources - amt < 0 ? 0 : _onHandResources - amt; }
	void					setLocation( Engine::Point2D<int> newLoc ) { _mapLocation = newLoc; }
	const Engine::Point2D<int>&		getLocation() const { return _mapLocation; }
	Engine::IncrementalRaster&		getResourcesRaster() { return _resources; }
	const Engine::IncrementalRaster&	getResourcesRaster() const { return _resources; }

	void		addAction( Action* a );
	Action*		availableActions( Problem::action_t actIndex ) { return _availableActions.at(actIndex); }
	const Action*	availableActions( Problem::action_t actIndex ) const { return _availableActions.at(actIndex); }

	unsigned	numAvailableActions() const { return _availableActions.size(); }

private:
	
	void	computeHash();

private:
	unsigned			_timeIndex;
	Engine::Point2D<int>		_mapLocation;
	// MRJ: This variable lies in the interval [0,100]
	int				_onHandResources;
	Engine::IncrementalRaster	_resources;
	Engine::HashKey			_hashKey;
	std::vector<Action*>		_availableActions;
};

inline std::ostream& operator<<( std::ostream& os, const HunterGathererMDPState& s )
{
	s.print(os);
	return os;	
}

}

#endif // HunterGathererMDPState.hxx
