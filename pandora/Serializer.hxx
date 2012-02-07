
#ifndef __Serializer_hxx__
#define __Serializer_hxx__

#include <hdf5.h>
#include <string>
#include <map>

namespace Engine
{

class Simulation;
class Agent;
class StaticRaster;
class Raster;
class World;

class Serializer
{
	typedef std::map< std::string, Raster> RastersMap;
	typedef std::map< std::string, StaticRaster> StaticRastersMap;
	std::string _fileName;

	hid_t _agentsFileId;
	hid_t _fileId;
	// this id is used to track the data set of the agent being serialized
	hid_t _currentAgentDatasetId;

	void serializeRaster( StaticRaster & raster, World & world, const std::string & datasetKey );
public:
	Serializer( const std::string & fileName = "data/simulation.h5");
	virtual ~Serializer();

	void init( Simulation & simulation, StaticRastersMap & staticRasters, RastersMap & rasters, World & world );
	void finish();

	void serializeAgent( Agent * agent, const int & step );
	void serializeAttribute( const std::string & name, const int & value );
	void serializeRaster( const std::string & key, Raster & raster, World & world, const int & step );
	void serializeStaticRaster( const std::string & key, StaticRaster & raster, World & world );
};

} // namespace Engine

#endif // __Serializer_hxx__

