#ifndef __HunterGathererMDPState_hxx__
#define __HunterGathererMDPState_hxx__

#include "Point2D.hxx"

namespace Gujarat
{

class HunterGathererMDPProblem;

class HunterGathererMDPState
{
public:
	// Constructors
	HunterGathererMDPState();
	HunterGathererMDPState(Engine::Point2D<int> loc, unsigned initResources);
	HunterGathererMDPState( const HunterGathererMPDState& s );
	const HunterGathererMDPState& operator=( const HunterGathererMDPState& s );
	
	~HunterGathererMDPState();

	size_t hash() const;
	bool	operator==( const HunterGathererMDPState& s );
	bool	operator!=( const HunterGathererMDPState& s );
	bool	operator<( const HunterGathererMDPState& s );

	void	increaseTimeIndex() { _timeIndex++; }
	void	addResources( int amt ) { _onHandResources = _onHandResources + amt > 100 ? 100 : _onHandResources + amt; }
	void	decreaseResources( int amt ) { _onHandResources = _onHandResources - amt < 0 ? 0 : _onHandResources - amt; }
	void	setLocation( Engined::Point2D<int> newLoc ) { _mapLocation = newLoc; }

private:
	unsigned		_timeIndex;
	Engine::Point2D<int>	_mapLocation;
	// MRJ: This variable lies in the interval [0,100]
	int			_onHandResources;

	friend HunterGathererMDPProblem;
};

}

#endif // HunterGathererMDPState.hxx
