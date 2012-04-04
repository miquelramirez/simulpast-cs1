
#ifndef __StaticRaster_hxx__
#define __StaticRaster_hxx__

#include "Point2D.hxx"
#include <vector>

namespace Engine
{
class World;

//! this class is used to load a static raster map. Values can't be modified, and it won't be serialized each time step (only one time)
class StaticRaster
{
protected:
	std::vector< std::vector<int> >_values;

	int _minValue;
	int _maxValue;
public:
	StaticRaster();
	virtual ~StaticRaster();

	// TODO not squared rasters
	//! changes raster size. Parameter 'size' represents the new dimesions for the raster area.
	virtual void resize( const Point2D<int> & size );
	//! Reads the value in the cell located by parameter "position". Returns -1 if "position" is out of the area of the raster.
	virtual const int & getValue( Point2D<int> position ) const;

	//! Returns size of the raster codifying the horizontal and vertical dimensions in a Point2D object. 
	Point2D<int> getSize() const;
	// load a GDAL file conforming World position
	void loadGDALFile( const std::string & fileName, World & world );
	// load an HDF5 adjusting the raster to its size
	void loadHDF5File( const std::string & fileName, const std::string & rasterName );
	// load an HDF5 conforming World position
	void loadHDF5File( const std::string & fileName, const std::string & rasterName, World & world );
	//! Initializes the components of vector '_values' to defaultValue, and to maxValue the ones from vector _maxValue.
	void setDefaultInitValues( int minValue, int maxValue, int defaultValue );	
	//! Assigns the value "value" to the cell located by parameter "position". Does nothing if "position" is out of the area of the raster.
	void setInitValue( Point2D<int> position, int value );
	//! Reads attribute _maxValue.
	const int & getMaxValue() const;
	//! Reads the '_minValue' attribute.
	const int & getMinValue() const;
}; 

} // namespace Engine

#endif // __StaticRaster_hxx__

