#ifndef __HunterGathererMDPState_hxx__
#define __HunterGathererMDPState_hxx__

#include "Point2D.hxx"
#include "IncrementalRaster.hxx"
#include "HashTable.hxx"

namespace Gujarat
{

class HunterGathererMDPProblem;

class HunterGathererMDPState
{
public:
	// Constructors, I don't want this to be ever invoked
	explicit HunterGathererMDPState();

	// The real one
	HunterGathererMDPState(Engine::Point2D<int> loc, unsigned initialOnHand, Engine::Raster& resourcesRaster);
	HunterGathererMDPState( const HunterGathererMDPState& s );
	
	~HunterGathererMDPState();

	unsigned	hash() const;
	bool		operator==( const HunterGathererMDPState& s );
	bool		operator!=( const HunterGathererMDPState& s );
	bool		operator<( const HunterGathererMDPState& s );

	void				increaseTimeIndex() { _timeIndex++; }
	void				addResources( int amt ) { _onHandResources = _onHandResources + amt > 100 ? 100 : _onHandResources + amt; }
	void				decreaseResources( int amt ) { _onHandResources = _onHandResources - amt < 0 ? 0 : _onHandResources - amt; }
	void				setLocation( Engine::Point2D<int> newLoc ) { _mapLocation = newLoc; }
	Engine::IncrementalRaster&		getResourcesRaster() { return _resources; }
	const Engine::IncrementalRaster&	getResourcesRaster() const { return _resources; }

private:
	
	void	computeHash();

private:
	unsigned			_timeIndex;
	Engine::Point2D<int>		_mapLocation;
	// MRJ: This variable lies in the interval [0,100]
	int				_onHandResources;
	Engine::IncrementalRaster	_resources;
	Engine::HashKey			_hashKey;
};

}

#endif // HunterGathererMDPState.hxx
