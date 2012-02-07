
#include "ProjectConfiguration.hxx"

#include "SimulationRecord.hxx"
#include "AgentConfiguration.hxx"
#include "RasterConfiguration.hxx"
#include "Exceptions.hxx"

#include <QMessageBox>

#include <tinyxml.h>

namespace GUI
{

ProjectConfiguration * ProjectConfiguration::_instance = 0;

ProjectConfiguration * ProjectConfiguration::instance()
{
	if(!_instance)
	{
		_instance = new ProjectConfiguration();
	}
	return _instance;
}

ProjectConfiguration::ProjectConfiguration() : _simulationRecord(0)
{
	reset();
}

ProjectConfiguration::~ProjectConfiguration()
{
	if(_simulationRecord)
	{
		delete _simulationRecord;
	}
	cleanConfigs();
}

void ProjectConfiguration::reset()
{
	_fileName = "unknown.cas";
	_simulationFileName = "unknown.h5";
	_resolution = 1;	
	
	if(_simulationRecord)
	{
		delete _simulationRecord;
		_simulationRecord = 0;
	}
	cleanConfigs();
}

void ProjectConfiguration::loadParameters( const std::string & fileName )
{
	reset();
	_fileName = fileName;

	TiXmlDocument fileProject;
	bool loadOkay = fileProject.LoadFile(_fileName);
	if(!loadOkay)
	{
		QMessageBox::information(0, "Failed to open project file", _fileName.c_str());
		return;
	}

	TiXmlElement * simulation = fileProject.FirstChildElement("simulation");
	_simulationFileName = simulation->Attribute("execution");
	_resolution = atoi(simulation->Attribute("resolution"));
}

bool ProjectConfiguration::loadSimulation()
{
	if(_simulationRecord)
	{
		delete _simulationRecord;
	}
	_simulationRecord = new Engine::SimulationRecord(_resolution);

	if(!_simulationRecord->loadHDF5(_simulationFileName))
	{
		delete _simulationRecord;
		_simulationRecord = 0;
		return false;
	}
	return true;
}

void ProjectConfiguration::store( const std::string & fileName )
{
	_fileName = fileName;
	store();
}

void ProjectConfiguration::store()
{
	TiXmlDocument fileProject;
	TiXmlDeclaration * declaration = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * simulation = new TiXmlElement( "simulation" );
	simulation->SetAttribute("execution", _simulationFileName);
	simulation->SetAttribute("resolution", _resolution);
	
	fileProject.LinkEndChild( declaration );
	fileProject.LinkEndChild( simulation );
	fileProject.SaveFile(_fileName);
}

void ProjectConfiguration::setResolution( const int & resolution )
{
	_resolution = resolution;
}

int ProjectConfiguration::getResolution()
{
	return _resolution;
}

void ProjectConfiguration::setSimulationFileName( const std::string & simulationFileName )
{
	_simulationFileName = simulationFileName;
}

const std::string & ProjectConfiguration::getSimulationFileName()
{
	return _simulationFileName;
}

bool ProjectConfiguration::alreadySaved()
{
	return _fileName.compare("unknown.cas");
}

Engine::SimulationRecord * ProjectConfiguration::getSimulationRecord()
{
	return _simulationRecord;
}

void ProjectConfiguration::cleanConfigs()
{
	for(AgentsConfigurationMap::iterator it=_agentsConfig.begin(); it!=_agentsConfig.end(); it++)
	{
		AgentConfiguration * agentConfig = it->second;
		delete agentConfig;
		it->second = 0;
	}
	_agentsConfig.clear();

	for(RastersConfigurationMap::iterator it=_rastersConfig.begin(); it!=_rastersConfig.end(); it++)
	{
		RasterConfiguration * rasterConfig = it->second;
		delete rasterConfig;
		it->second = 0;
	}
	_rastersConfig.clear();
}

void ProjectConfiguration::resetConfigs()
{
	cleanConfigs();
	
	Engine::SimulationRecord * simulationRecord = ProjectConfiguration::instance()->getSimulationRecord();
	if(!simulationRecord)
	{
		return;
	}

	for(Engine::SimulationRecord::AgentTypesMap::iterator itType = simulationRecord->beginTypes(); itType!=simulationRecord->endTypes(); itType++)
	{
		_agentsConfig.insert(make_pair( itType->first, new AgentConfiguration() ));
	}	
	
	std::list<std::string> rastersNames;
	for(Engine::SimulationRecord::RasterMap::iterator itRaster = simulationRecord->beginRasters(); itRaster!=simulationRecord->endRasters(); itRaster++)
	{
		int minValue = itRaster->second[0].getMinValue();
		int maxValue = itRaster->second[0].getMaxValue();
		std::cout << itRaster->first << " dynamic, min: " << minValue << " max: " << maxValue << std::endl;
		_rastersConfig.insert(make_pair( itRaster->first, new RasterConfiguration(minValue,maxValue) ));
		rastersNames.push_back(itRaster->first);
	}

	for(Engine::SimulationRecord::StaticRasterMap::iterator itRaster = simulationRecord->beginStaticRasters(); itRaster!=simulationRecord->endStaticRasters(); itRaster++)
	{
		int minValue = itRaster->second.getMinValue();
		int maxValue = itRaster->second.getMaxValue();
		std::cout << itRaster->first << " static, min: " << minValue << " max: " << maxValue << std::endl;
		_rastersConfig.insert(make_pair( itRaster->first, new RasterConfiguration(minValue,maxValue)));
		rastersNames.push_back(itRaster->first);
	}
	_config3D.setRastersList(rastersNames);
}

void ProjectConfiguration::updateAgentConfig( const std::string & type, const AgentConfiguration & config )
{
	AgentsConfigurationMap::iterator it = _agentsConfig.find(type);
	if(it==_agentsConfig.end())
	{
		std::stringstream oss;
		oss << "ProjectConfiguration::agentConfigured - config not found for agent type: " << type;
		throw Engine::Exception(oss.str());
		return;
	}
	else
	{
		AgentConfiguration * agentConfig = it->second;
		delete agentConfig;
		it->second = new AgentConfiguration(config);
	}
}

void ProjectConfiguration::updateRasterConfig( const std::string & type, const RasterConfiguration & config )
{
	RastersConfigurationMap::iterator it = _rastersConfig.find(type);
	if(it==_rastersConfig.end())
	{
		std::stringstream oss;
		oss << "ProjectConfiguration::rasterConfigured - config not found for raster type: " << type;
		throw Engine::Exception(oss.str());
		return;
	}
	else
	{
		RasterConfiguration * rasterConfig = it->second;
		delete rasterConfig;
		it->second = new RasterConfiguration(config);
	}
}

AgentConfiguration * ProjectConfiguration::getAgentConfig( const std::string & type )
{
	AgentsConfigurationMap::iterator it = _agentsConfig.find(type);
	if(it==_agentsConfig.end())
	{
		std::stringstream oss;
		oss << "ProjectConfiguration::getAgentConfig - config not found for agent type: " << type; 
		throw Engine::Exception(oss.str());
		return 0;
	}
	return it->second;
}

RasterConfiguration * ProjectConfiguration::getRasterConfig( const std::string & type )
{
	RastersConfigurationMap::iterator it = _rastersConfig.find(type);
	if(it==_rastersConfig.end())
	{
		std::stringstream oss;
		oss << "ProjectConfiguration::getRasterConfig - config not found for agent type: " << type; 
		throw Engine::Exception(oss.str());
		return 0;
	}
	return it->second;
}

Configuration3D & ProjectConfiguration::getConfig3D()
{
	return _config3D;
}

} // namespace GUI

