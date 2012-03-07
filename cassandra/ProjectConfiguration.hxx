
#ifndef __ProjectConfiguration_hxx__
#define __ProjectConfiguration_hxx__

#include <string>
#include <map>
#include "Configuration3D.hxx"

class TiXmlElement;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{

class AgentConfiguration;
class RasterConfiguration;

class ProjectConfiguration
{
public:	
	typedef std::map<std::string, AgentConfiguration * > AgentsConfigurationMap;
	typedef std::map<std::string, RasterConfiguration * > RastersConfigurationMap;

private:
	static ProjectConfiguration * _instance;
	
	std::string _fileName;

	std::string _simulationFileName;
	int _resolution;

	Engine::SimulationRecord * _simulationRecord;
	
	ProjectConfiguration();

	AgentsConfigurationMap _agentsConfig;
	RastersConfigurationMap _rastersConfig;
	Configuration3D _config3D;
	
	void cleanConfigs();

	TiXmlElement * storeAgentsConfigs();
	TiXmlElement * storeRastersConfigs();

	void loadAgentsConfigs( TiXmlElement * agents );
	void loadRastersConfigs( TiXmlElement * rasters );

	// this method checks whether default configs are needed for a simulation
	// if this is the case, they are created
	void checkConfigs();

public:
	static ProjectConfiguration * instance();
	virtual ~ProjectConfiguration();

	void reset();

	void loadProject( const std::string & fileName );
	// this function is used to store the project to a fileName different from _fileName
	void storeProject( const std::string & fileName );
	// this function is used to store the project to _fileName
	void storeProject();

	void setResolution( const int & resolution );
	int getResolution();

	const std::string & getSimulationFileName();
	void setSimulationFileName( const std::string & simulationFileName );	

	bool alreadySaved();

	Engine::SimulationRecord * getSimulationRecord();
	// returns true if simulation was correctly loaded
	bool loadSimulation();

	void updateAgentConfig( const std::string & type, const AgentConfiguration & config );
	void updateRasterConfig( const std::string & type, const RasterConfiguration & config );
	AgentConfiguration * getAgentConfig( const std::string & type );
	RasterConfiguration * getRasterConfig( const std::string & type );
	Configuration3D & getConfig3D();

};

} // namespace GUI

#endif // __ProjectConfiguration_hxx__

