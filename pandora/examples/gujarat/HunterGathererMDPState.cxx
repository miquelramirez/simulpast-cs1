#include "HunterGathererMDPState.hxx"

namespace Gujarat
{

HunterGathererMDPState::HunterGathererMDPState()
	: _timeIndex ( 0 ), _onHandResources( 0 )
{
}

HunterGathererMDPState::HunterGathererMDPState( const HunterGathererMDPState& s )
{
	_timeIndex = s._timeIndex;
	_mapLocation = s._mapLocation;
	_onHandResources = s._onHandResources;
}

const HunterGathererMDPState& HunterGathererMDPState::operator=( const HunterGathererMDPState& s )
{
	_timeIndex = s._timeIndex;
	_mapLocation = s._mapLocation;
	_onHandResources = s._onHandResources;

	return *this;
}

HunterGathererMDPState::~HunterGathererMDPState()
{
}

size_t HunterGathererMDPState::hash() const
{
}

bool	HunterGathererMDPState::operator==( const HunterGathererMDPState& s )
{
}

bool	HunterGathererMDPState::operator!=( const HunterGathererMDPState& s )
{
}

bool	HunterGathererMDPState::operator<( const HunterGathererMDPState& s )
{
}


}
