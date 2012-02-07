
#ifndef __ProjectConfiguration_hxx__
#define __ProjectConfiguration_hxx__

#include <string>
#include <map>
#include "Configuration3D.hxx"

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

public:
	static ProjectConfiguration * instance();
	virtual ~ProjectConfiguration();

	void reset();

	void loadParameters( const std::string & fileName );
	// this function is used to store the project to a fileName different from _fileName
	void store( const std::string & fileName );
	// this function is used to store the project to _fileName
	void store();

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
	void resetConfigs();
};

} // namespace GUI

#endif // __ProjectConfiguration_hxx__

