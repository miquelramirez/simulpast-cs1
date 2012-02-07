
#ifndef __SimulationRecord_hxx__
#define __SimulationRecord_hxx__

#include <vector>
#include <string>
#include <map>
#include <hdf5.h>

#include "Raster.hxx"
#include "StaticRaster.hxx"
#include "AgentRecord.hxx"
#include "Point2D.hxx"

namespace Engine 
{

class SimulationRecord
{
public:
	// TODO programar funció d'equivalencia a agent record i convertir en llista
	typedef std::map<std::string, AgentRecord * > AgentRecordsMap;
	typedef std::map<std::string, int > ValuesMap;
	typedef std::vector<Engine::Raster> RasterHistory;
	typedef std::map<std::string, RasterHistory> RasterMap;
	typedef std::map<std::string, Engine::StaticRaster> StaticRasterMap;
	typedef std::map<std::string, AgentRecordsMap > AgentTypesMap;
private:

	std::string _name;
	RasterMap _resources;
	StaticRasterMap _staticRasters;
	AgentTypesMap _types;
	int _numSteps;
	// the step currently being loaded
	int _loadingStep;
	int _resolution;

	// we need to know min and max values for each state in order to paint agents 
	ValuesMap _minAttributeValues;
	ValuesMap _maxAttributeValues;

	int _size;

	// we need this function in order to be called by H5Giterate. It must be static to match the C call signature
	static herr_t registerAgentStep( hid_t loc_id, const char *name, void *opdata );

	float _loadingPercentageDone;
	std::string _loadingState;
public:
	SimulationRecord( int resolution = 1 );
	virtual ~SimulationRecord();

	// the real method, called from registerAgentStep
	void registerAgent( hid_t loc_id, const char * name );
	bool loadHDF5( const std::string & fileName, const bool & loadRasters=true, const bool & loadAgents=true);
	
	RasterHistory & getRasterHistory( const std::string & key );
	Engine::Raster & getDynamicRaster( const std::string & key, const int & step );
	Engine::StaticRaster & getStaticRaster( const std::string & key );
	Engine::StaticRaster & getRasterTmp( const std::string & key, const int & step );

	const std::string & getName();
	int getNumSteps();
	int getResolution();	
	
	AgentRecordsMap::iterator beginAgents( const std::string & type );
	AgentRecordsMap::iterator beginAgents( AgentTypesMap::iterator & it );
	AgentRecordsMap::iterator endAgents( const std::string & type );
	AgentRecordsMap::iterator endAgents( AgentTypesMap::iterator & it );

	AgentTypesMap::iterator beginTypes();
	AgentTypesMap::iterator endTypes();

	RasterMap::iterator beginRasters();
	RasterMap::iterator endRasters();

	StaticRasterMap::iterator beginStaticRasters();
	StaticRasterMap::iterator endStaticRasters();

	AgentRecord * getAgentAtPosition( int step, const Engine::Point2D<int> & position );

	// TODO make a different class
	double getMean( const std::string & type, const std::string & state, int step );
	double getSum( const std::string & type, const std::string & state, int step );
	
	int getMinValueForState( const std::string & state );
	int getMaxValueForState( const std::string & state );
	int getSize();
	const float & getLoadingPercentageDone() const;
	const std::string & getLoadingState() const;
};

} // namespace Engine 

#endif // __SimulationRecord_hxx__

