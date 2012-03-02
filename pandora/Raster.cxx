
#include "Raster.hxx"
#include "Exceptions.hxx"

#include <sstream>
#include <iostream>
#include <limits>

namespace Engine
{

Raster::Raster()
{
}

Raster::~Raster()
{
}

void Raster::resize( const Point2D<int> & size )
{
	StaticRaster::resize(size);
	_maxValues.resize(size._x);
	// TODO explore how to improve performance with STL algorithms
	// for_each(_valuesField.begin(),_valuesField.end(),resize(_matrixSize));
	for(int i=0; i<size._x; i++)
	{
		_maxValues[i].resize(size._y);
	}
}

void Raster::updateRasterIncrement()
{
	for(int i=0; i<_values.size(); i++)
	{
		for(int j=0; j<_values.size(); j++)
		{
			if(_values[i][j] < _maxValues[i][j])
			{
				_values[i][j]++;
			}
		}
	}
}

void Raster::updateRasterToMaxValues()
{
	// TODO
	// could it be done with a memcopy?
	// use std::copy(_maxValues.begin(), _maxValues.end(), _values) 
	// how often that method is called?
	
	// std::copy is deep oriented.
	std::copy(_maxValues.begin(), _maxValues.end(), _values.begin());
	/*
	for(int i=0; i<_values.size(); i++)
	{
	
		for(int j=0; j<_values[i].size(); j++)
		{
			_values[i][j] = _maxValues[i][j];
		}
	}
	*/
}

int Raster::getMaxValueAt( Point2D<int> position )
{
	if(position._x<0 || position._x>=_maxValues.size())
	{
		std::stringstream oss;
		oss << "Raster::getMaxValueAt - " << position << " x out of bounds: " << _maxValues.size();
		throw Exception(oss.str());
		return -1;
	}
	if(position._y<0 || position._y>=_maxValues[position._x].size())
	{
		std::stringstream oss;
		oss << "Raster::getMaxValueAt - " << position << " y out of bounds: " << _maxValues.size() << "/" << _maxValues[position._x].size();
		throw Exception(oss.str());
		return -1;
	}	
	return _maxValues[position._x][position._y];
}

void Raster::setValue( Point2D<int> position, int value )
{
	if(value>_maxValue)
	{
		std::stringstream oss;
		oss << "Raster::setValue - value: " << value << " bigger than max value: " << _maxValue << " at position: " << position;
		throw Exception(oss.str());

		return;
	}
	if(position._x<0 || position._x>=_values.size())
	{
		std::stringstream oss;
		oss << "Raster::setValue - " << position << " x out of bounds: " << _values.size();
		throw Exception(oss.str());

		return;
	}
	if(position._y<0 || position._y>=_values[position._x].size())
	{
		std::stringstream oss;
		oss << "Raster::setValue - " << position << " y out of bounds: " << _values.size() << "/" << _values[position._x].size();
		throw Exception(oss.str());
		return;
	}
	if(value>_maxValues[position._x][position._y])
	{
		std::stringstream oss;
		oss << "Raster::setValue - value: " << value << " bigger than max value: " << _maxValues[position._x][position._y] << " at position: " << position;
		throw Exception(oss.str());
	}
	_values[position._x][position._y] = value;
}

void Raster::setMaxValue( Point2D<int> position, int value )
{
	if(value>_maxValue)
	{
		std::stringstream oss;
		oss << "Raster::setMaxValue - value: " << value << " bigger than max value: " << _maxValue << " at pos: " << position;
		throw Exception(oss.str());

		return;
	}

	if(position._x<0 || position._x>=_maxValues.size())
	{
		std::stringstream oss;
		oss << "Raster::setMaxValue- " << position << " x out of bounds: " << _maxValues.size();
		throw Exception(oss.str());

		return;
	}
	if(position._y<0 || position._y>=_maxValues[position._x].size())
	{
		std::stringstream oss;
		oss << "Raster::setMaxValue- " << position << " y out of bounds: " << _maxValues.size() << "/" << _maxValues[position._x].size();
		throw Exception(oss.str());
		return;
	}	
	_maxValues[position._x][position._y] = value;
}

void Raster::setMaxValue( const int & maxValue )
{
	_maxValue = maxValue;
}

void Raster::setMinValue( const int & minValue )
{
	_minValue = minValue;
}

void Raster::updateCurrentMinMaxValues()
{
	_currentMinValue = std::numeric_limits<int>::max();
	_currentMaxValue = std::numeric_limits<int>::min();
	for ( unsigned i = 0; i < _values.size(); i++  )
		for ( unsigned j = 0; j < _values[i].size(); j++ )
		{
			_currentMaxValue = ( _values[i][j] > _currentMaxValue ? _values[i][j] : _currentMaxValue );
			_currentMinValue = ( _values[i][j] < _currentMinValue ? _values[i][j] : _currentMinValue );
		}
}

void Raster::setInitValues( int minValue, int maxValue, int defaultValue )
{
	_minValue = _currentMinValue = minValue;
	_maxValue = _currentMaxValue = maxValue;
	for(int i=0; i<_values.size(); i++)
	{
		for(int j=0; j<_values[i].size(); j++)
		{
			setMaxValue(Point2D<int>(i,j), _maxValue);
			setValue(Point2D<int>(i,j), defaultValue);			
		}
	}
}

void Raster::loadGDALFile( const std::string & fileName, World & world )
{
	StaticRaster::loadGDALFile(fileName, world);
	std::copy(_values.begin(), _values.end(), _maxValues.begin());
}

} // namespace Engine

