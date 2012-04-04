
#ifndef __Raster_hxx__
#define __Raster_hxx__

#include "Point2D.hxx"
#include "StaticRaster.hxx"

namespace Engine
{

//! Raster adds mechanisms to modify the values of the raster map. It is serialized each time step.
class Raster : public StaticRaster
{
	std::vector< std::vector<int> >	_maxValues;
	int				_currentMaxValue;
	int				_currentMinValue;
public:
	Raster();
	virtual ~Raster();

	// parameters: starting pos and size in matrix to grow
	//! Increases each cell value by 1 if it is under the maximum allowed.
	void updateRasterIncrement();
	// parameters: starting pos and size in matrix to grow
	//! Assigns to each cell in raster the max value allowed for it.
	void updateRasterToMaxValues();
	
	//! Reads the maximum allowed value in the cell located by parameter "position". Returns -1 if "position" is out of the area of the raster.
	int getMaxValueAt( Point2D<int> position );
	
	//! Assigns the value "value" to the cell located by parameter "position". Does nothing if "position" is out of the area of the raster.
	virtual void setValue( Point2D<int> position, int value );
	//! Changes the maximum value allowed in the cell located by parameter "position" to the new amount "value". Does nothing if "position" is out of the area of the raster.
	void setMaxValue( Point2D<int> position, int value );
	
	//! Initializes the components of vector '_values' to defaultValue, and to maxValue the ones from vector _maxValue.
	void setInitValues( int minValue, int maxValue, int defaultValue );
	//! Sets new value for attribute maxValue.
	void setMaxValue( const int & maxValue);
	//! Sets new value for attribute minValue.
	void setMinValue( const int & minValue);
	void resize( const Point2D<int> & size );
	
	void loadGDALFile( const std::string & fileName, World & world );
	void updateCurrentMinMaxValues();

	int  getCurrentMinValue() const { return _currentMinValue; }
	int  getCurrentMaxValue() const { return _currentMaxValue; }
};

} // namespace Engine

#endif // __Raster_hxx__

