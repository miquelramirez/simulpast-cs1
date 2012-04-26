#include "HunterGathererMDPState.hxx"

namespace Gujarat
{

HunterGathererMDPState::HunterGathererMDPState()
	: _timeIndex(0), _onHandResources(0)
{
}

HunterGathererMDPState::HunterGathererMDPState(Engine::Point2D<int> loc, int initResources, 
	const Engine::Raster& resourcesRaster, int divider )
	: _timeIndex(0), _mapLocation( loc ), _onHandResources( initResources/divider ), _resources( resourcesRaster ),
	_resourcesDivider( divider ), _daysStarving( 0 )
{
	computeHash();
}

HunterGathererMDPState::HunterGathererMDPState( const HunterGathererMDPState& s )
	: _timeIndex( s._timeIndex ), _mapLocation( s._mapLocation ), _onHandResources( s._onHandResources ),
	_resources( s._resources ), _resourcesDivider( s._resourcesDivider ), _daysStarving( s._daysStarving )
{
	computeHash();
	
	for ( unsigned k = 0; k < s._availableActions.size(); k++ )
	{
		addAction( s._availableActions[k]->copy() );
	}
}

const HunterGathererMDPState& HunterGathererMDPState::operator=( const HunterGathererMDPState& s )
{
	_timeIndex = s._timeIndex;
	_mapLocation = s._mapLocation;
	_onHandResources = s._onHandResources;
	_resources = s._resources;
	_resourcesDivider = s._resourcesDivider;
	_daysStarving = s._daysStarving;	

	computeHash();
	
	for ( unsigned k = 0; k < s._availableActions.size(); k++ )
	{
		addAction( s._availableActions[k]->copy() );
	}

	return *this;
}

void	HunterGathererMDPState::initializeSuccessor( HunterGathererMDPState& s ) const
{
	s._timeIndex = _timeIndex;
	s._mapLocation = _mapLocation;
	s._onHandResources = _onHandResources;
	s._resources = _resources;
	s._resourcesDivider = _resourcesDivider;
	s._daysStarving = _daysStarving;
}

HunterGathererMDPState::~HunterGathererMDPState()
{
	for ( unsigned k = 0; k < _availableActions.size(); k++ )
		delete _availableActions[k];
}

void	HunterGathererMDPState::addAction( Action* a )
{
	_availableActions.push_back(a);
}

void	HunterGathererMDPState::computeHash()
{
	_hashKey.add( _timeIndex );
	_hashKey.add( _mapLocation._x );
	_hashKey.add( _mapLocation._y );
	_hashKey.add( _onHandResources );
	_hashKey.add( _daysStarving );

	for ( Engine::IncrementalRaster::ChangeIterator it = _resources.firstChange();
		it != _resources.endOfChanges(); it++ )
	{
		const Engine::Point2D<int>& p = it->first;
		_hashKey.add( p._x );
		_hashKey.add( p._y );
		_hashKey.add( it->second );
	}
}

unsigned HunterGathererMDPState::hash() const
{
	return _hashKey.code(); 
}

bool	HunterGathererMDPState::operator==( const HunterGathererMDPState& s ) const
{
	return ( _timeIndex == s._timeIndex ) &&
			( _onHandResources == s._onHandResources ) &&
			( _mapLocation == s._mapLocation ) &&
			( _resources == s._resources );
}

bool	HunterGathererMDPState::operator!=( const HunterGathererMDPState& s ) const
{
	return ( _timeIndex != s._timeIndex ) &&
			( _onHandResources != s._onHandResources ) &&
			( _mapLocation != s._mapLocation ) &&
			( _resources != s._resources );
}

bool	HunterGathererMDPState::operator<( const HunterGathererMDPState& s ) const
{
	return  ( _timeIndex < s._timeIndex ) ||
			( ( _timeIndex == s._timeIndex ) && ( _onHandResources < s._onHandResources ) ) ||
			( ( _timeIndex == s._timeIndex ) && ( _onHandResources == s._onHandResources) &&
			  ( _mapLocation < s._mapLocation) ) ||
			( ( _timeIndex == s._timeIndex ) && ( _onHandResources == s._onHandResources) &&
			  ( _mapLocation == s._mapLocation) && ( _resources < s._resources ) );
}

void	HunterGathererMDPState::print( std::ostream& os ) const
{
	os << "<addr = " << this << ", ";
	os << "loc=(" << _mapLocation._x << ", " << _mapLocation._y << "), ";
	os << "res=" << _onHandResources << ", ";
	os << "t=" << _timeIndex << ", ";
	os << "A(s)=" << _availableActions.size() << ">" << std::endl;	
}


}
