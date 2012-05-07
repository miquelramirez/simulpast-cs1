
#ifndef __RasterLoader_hxx__
#define __RasterLoader_hxx__

#include <string>

namespace Engine
{

class StaticRaster;
class World;

class RasterLoader
{
	static RasterLoader * _instance;
protected:
	RasterLoader();
public:
	static RasterLoader * instance();
	virtual ~RasterLoader();

	// load a GDAL file conforming World position
	void fillGDALRaster( StaticRaster & raster, const std::string & fileName, World * world );
	// load an HDF5 conforming adjusting raster to data, or to World position if not null
	void fillHDF5Raster( StaticRaster & raster, const std::string & fileName, const std::string & rasterName, World * world = 0);
	// load a raster file from a GRASS database conforming adjusting raster to data, or to World position if not null
	// Simulation MUST be executed from a Grass instance, and raster must exist in current location/mapset
	void fillGrassCellRaster( StaticRaster & raster, const std::string & rasterName, World * world = 0);
}; 

} // namespace Engine

#endif // __RasterLoader_hxx__

