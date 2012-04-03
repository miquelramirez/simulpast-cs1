#include "HunterGathererMDPState.hxx"

namespace Gujarat
{

HunterGathererMDPState::HunterGathererMDPState()
	: _timeIndex(0), _onHandResources(0), _resources( *((Engine::Raster*)NULL) )
{
}

HunterGathererMDPState::HunterGathererMDPState(Engine::Point2D<int> loc, int initResources, const Engine::Raster& resourcesRaster)
	: _timeIndex(0), _mapLocation( loc ), _onHandResources( initResources ), _resources( resourcesRaster ) 
{
	computeHash();
}

HunterGathererMDPState::HunterGathererMDPState( const HunterGathererMDPState& s )
	: _timeIndex( s._timeIndex ), _mapLocation( s._mapLocation ), _onHandResources( s._onHandResources ),
	_resources( s._resources )
{
	computeHash();
}

HunterGathererMDPState::~HunterGathererMDPState()
{
	for ( unsigned k = 0; k < _availableActions.size(); k++ )
		delete _availableActions[k];
}



void	HunterGathererMDPState::computeHash()
{
	_hashKey.add( _timeIndex );
	_hashKey.add( _mapLocation._x );
	_hashKey.add( _mapLocation._y );
	_hashKey.add( _onHandResources );

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

bool	HunterGathererMDPState::operator==( const HunterGathererMDPState& s )
{
	return ( _timeIndex == s._timeIndex ) &&
			( _onHandResources == s._onHandResources ) &&
			( _mapLocation == s._mapLocation ) &&
			( _resources == s._resources );
}

bool	HunterGathererMDPState::operator!=( const HunterGathererMDPState& s )
{
	return ( _timeIndex != s._timeIndex ) &&
			( _onHandResources != s._onHandResources ) &&
			( _mapLocation != s._mapLocation ) &&
			( _resources != s._resources );
}

bool	HunterGathererMDPState::operator<( const HunterGathererMDPState& s )
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
	os << "<loc=(" << _mapLocation._x << ", " << _mapLocation._y << "), ";
	os << "res=" << _onHandResources << ", ";
	os << "t=" << _timeIndex << ">" << std::endl;	
}


}
