#include "IncrementalRaster.hxx"
#include <limits>

namespace Engine 
{

IncrementalRaster::IncrementalRaster()
{
}

IncrementalRaster::IncrementalRaster( const Raster& baseRaster )
	: _baseRaster( &baseRaster )
{
	_currentMinValue = _baseRaster->getCurrentMinValue();
	_currentMaxValue = _baseRaster->getCurrentMaxValue();
}

IncrementalRaster::IncrementalRaster( const IncrementalRaster& other ) 
	: _changes( other._changes ), _baseRaster( other._baseRaster )
{
	_currentMinValue = other._currentMinValue;
	_currentMaxValue = other._currentMaxValue;
}

IncrementalRaster::~IncrementalRaster()
{
}

void 	IncrementalRaster::resize( const Point2D<int>& size )
{
}

void	IncrementalRaster::setValue( Point2D<int> pos, int value )
{
	_changes[pos] = value;
}

const int&	IncrementalRaster::getValue( Point2D<int> pos ) const
{
	ChangeTable::const_iterator it = _changes.find( pos );
	if ( it == _changes.end() )
		return _baseRaster->getValue( pos );
	return it->second;
}

/*
void	IncrementalRaster::updateCurrentMinMaxValues()
{
	_currentMinValue = std::numeric_limits<int>::max();
	_currentMaxValue = std::numeric_limits<int>::min();
	Point2D<int>	dims = _baseRaster->getSize();

	for ( int i = 0; i < dims._x; i++  )
		for ( int j = 0; j < dims._y; j++ )
		{
			int v = getValue( Point2D<int>( i, j ) );
			_currentMaxValue = ( v > _currentMaxValue ? v : _currentMaxValue );
			_currentMinValue = ( v < _currentMinValue ? v : _currentMinValue );
		}
}
*/

bool	IncrementalRaster::operator==( const IncrementalRaster& other ) const
{
	for ( ChangeIterator i = firstChange(); 
		i != endOfChanges(); i++ )
	{
		ChangeIterator j = other._changes.find( i->first );
		if ( j == other.endOfChanges() ) return false;
		if ( i->second != j->second ) return false;
	}
	return true;
}

}
