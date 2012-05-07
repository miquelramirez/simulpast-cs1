
#include "SimulationRecord.hxx"
#include "RasterLoader.hxx"
#include "Exceptions.hxx"

#include <hdf5.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <limits>

namespace Engine 
{

SimulationRecord::SimulationRecord( int resolution ) : _name("unknown"), _numSteps(0), _loadingStep(0), _resolution(resolution), _loadingPercentageDone(0.0f), _loadingState("no load")
{	
}

SimulationRecord::~SimulationRecord()
{
}

bool SimulationRecord::loadHDF5( const std::string & fileName, const bool & loadRasters, const bool & loadAgents )
{
	_loadingPercentageDone = 0.0f;
	// TODO it deletes agent records?
	for(AgentTypesMap::iterator it = _types.begin(); it!=_types.end(); it++)
	{
		it->second.clear();
	}
	_types.clear();

	_resources.clear();

	_loadingState = "loading file: "+fileName+"...";
	_name = fileName;
	hid_t fileId = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	//std::cout << "FILE ID: " << fileId << " with path: " << fileName.c_str() << std::endl;
	if(fileId<0)
	{
		return false;
	}

	hid_t datasetId = H5Dopen(fileId, "global", H5P_DEFAULT); 
	hid_t attributeId = H5Aopen_name(datasetId, "numSteps");
	H5Aread(attributeId, H5T_NATIVE_INT, &_numSteps);
	H5Aclose(attributeId);

	int numStepsToLoad = 1+_numSteps/_resolution;
	// we need this information in order to open agents records
	attributeId = H5Aopen_name(datasetId, "numTasks");	
	int numTasks= 0;
	H5Aread(attributeId, H5T_NATIVE_INT, &numTasks);
	H5Aclose(attributeId);
	if(numTasks==0)
	{
		H5Dclose(datasetId);
		H5Fclose(fileId);
		// TODO throw exception
		return false;
	}

	attributeId = H5Aopen_name(datasetId, "size");	
	_size = 0;
	H5Aread(attributeId, H5T_NATIVE_INT, &_size );
	H5Aclose(attributeId);

	H5Dclose(datasetId);


	_loadingPercentageDone = 10.0f;
	// TODO refactor and split in two different methods
	if(loadRasters)
	{
		// static rasters
		hid_t staticRasterNamesDatasetId = H5Dopen(fileId, "staticRasters", H5P_DEFAULT);

		int numStaticRasters = H5Aget_num_attrs(staticRasterNamesDatasetId);
		for(int i=0; i<numStaticRasters; i++)
		{
			char nameAttribute[256];
			attributeId= H5Aopen_idx(staticRasterNamesDatasetId, i);
			H5Aget_name(attributeId, 256, nameAttribute);
			_staticRasters.insert( make_pair( std::string(nameAttribute), StaticRaster()));
			H5Aclose(attributeId);
		}
		H5Dclose(staticRasterNamesDatasetId);

		_loadingPercentageDone = 15.0f;
		for(StaticRasterMap::iterator it=_staticRasters.begin(); it!=_staticRasters.end(); it++)
		{
			_loadingState = "loading static raster: "+it->first;
			RasterLoader::instance()->fillHDF5Raster(it->second, fileName, it->first );
		}

		// dynamic rasters
		hid_t rasterNamesDatasetId = H5Dopen(fileId, "rasters", H5P_DEFAULT);
		int numRasters = H5Aget_num_attrs(rasterNamesDatasetId);
		if(numRasters!=0)
		{
			for(int i=0; i<numRasters; i++)
			{
				char nameAttribute[256];
				attributeId= H5Aopen_idx(rasterNamesDatasetId, i);
				H5Aget_name(attributeId, 256, nameAttribute);
				_resources.insert( make_pair( std::string(nameAttribute), RasterHistory()));
				getRasterHistory(nameAttribute).resize(numStepsToLoad);
				H5Aclose(attributeId);
			}
			H5Dclose(rasterNamesDatasetId);

			_loadingPercentageDone = 20.0f; // from 20 to 50
			float increase = 30.0f/(numRasters*numStepsToLoad);
			for(RasterMap::iterator it=_resources.begin(); it!=_resources.end(); it++)
			{
				_loadingState = "loading dynamic raster: "+it->first;
				int maxValue = std::numeric_limits<int>::min();
				int minValue = std::numeric_limits<int>::max();
				for(int i=0; i<=_numSteps; i=i+_resolution)
				{
					std::stringstream line;
					line << "loading raster " << it->first << " - step: " << i << "/" << _numSteps;
					_loadingState = line.str();

					std::ostringstream oss;
					oss << "/" << it->first << "/step" << i;
					hid_t dset_id = H5Dopen(fileId, oss.str().c_str(), H5P_DEFAULT);
					hid_t dataspaceId = H5Dget_space(dset_id);
					hsize_t dims[2];
					H5Sget_simple_extent_dims(dataspaceId, dims, NULL);
					H5Sclose(dataspaceId);

					int * dset_data = (int*)malloc(sizeof(int)*dims[0]*dims[1]);
					
					// squared	
					Engine::Raster & raster = it->second[i/_resolution];
					raster.resize(Engine::Point2D<int>(dims[0], dims[1]));
					// TODO max value!
					raster.setInitValues(std::numeric_limits<int>::min(),std::numeric_limits<int>::max(), 0);

					H5Dread(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dset_data);
					H5Dclose(dset_id);

					for(int i=0; i<dims[0]; i++)
					{
						for(int j=0; j<dims[1]; j++)
						{
							int index = i+j*dims[0];
							int value = dset_data[index];
							if(value>raster.getMaxValue())
							{
								raster.setMaxValue(value);
							}
							if(value>maxValue)
							{
								maxValue = value;
							}
							if(value<raster.getMinValue())
							{
								raster.setMinValue(value);
							}
							if(value<minValue)
							{
								minValue = value;
							}
							raster.setMaxValue(Engine::Point2D<int>(i,j), value);
							raster.setValue(Engine::Point2D<int>(i,j), value); 
						}
					}
					free(dset_data);
					_loadingPercentageDone += increase;
				}
				// setting max value for the entire simulation
				for(int i=0; i<=_numSteps; i=i+_resolution)
				{
					Engine::Raster & raster = it->second[i/_resolution];
					raster.setMaxValue(maxValue);
					raster.setMinValue(minValue);
				}
			}
		}
	}
	H5Fclose(fileId);

	_loadingPercentageDone = 50.0f;
	_loadingState = "loading agents";
	if(loadAgents)
	{
		unsigned int filePos = fileName.find_last_of("/");
		std::string path = fileName.substr(0,filePos+1);

		// we need to store in numsteps the working step, in order to access it from H5Giterate
		if(numStepsToLoad!=0 && numTasks!=0)
		{
			float increase = 50.0f/float(numTasks*numStepsToLoad);
			for(int i=0; i<numTasks; i++)
			{
				// agents
				std::ostringstream agentsFileName;
				agentsFileName << path << "agents-" << i<< ".abm";

				hid_t agentsFileId = H5Fopen(agentsFileName.str().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

				for(_loadingStep=0; _loadingStep<=_numSteps; _loadingStep=_loadingStep+_resolution)
				{
					std::stringstream line;
					line << "loading agents of task: "<< i+1 << "/" << numTasks << " - step: " << _loadingStep << "/" << _numSteps;
					_loadingState = line.str();
					std::ostringstream oss;
					oss << "/step" << _loadingStep;
					H5Giterate(agentsFileId, oss.str().c_str(), 0, registerAgentStep, this);
					_loadingPercentageDone += increase;
				}
				H5Fclose(agentsFileId);
			}
		}
	}

	_loadingState = "no loading";
	_loadingPercentageDone = 100.0f;
	return true;
}
	
SimulationRecord::RasterHistory & SimulationRecord::getRasterHistory( const std::string & key )
{
	RasterMap::iterator it = _resources.find(key);
	// the key does not exists
	if(it==_resources.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getRasterHistory - searching for unknown key: " << key << " in resource field histories";
		throw Engine::Exception(oss.str());
	}
	return it->second;
}

Engine::StaticRaster & SimulationRecord::getRasterTmp( const std::string & key, const int & step )
{
	StaticRasterMap::iterator itS = _staticRasters.find(key);
	// the key does not exists
	if(itS!=_staticRasters.end())
	{
		return itS->second;
	}
	RasterHistory & resourceHistory = getRasterHistory(key);
	int index = step/_resolution;
	if(index<0 || index>=resourceHistory.size())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getRasterTmp - asking for key: " << key << " and step: " << step << "out of bounds, having: " << resourceHistory.size()*_resolution << " steps";
		throw Engine::Exception(oss.str());

	}
	return resourceHistory[index];
}

Engine::Raster & SimulationRecord::getDynamicRaster(  const std::string & key, const int & step )
{
	RasterHistory & resourceHistory = getRasterHistory(key);
	int index = step/_resolution;
	if(index<0 || index>=resourceHistory.size())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getDynamicRaster - asking for key: " << key << " and step: " << step << "out of bounds, having: " << resourceHistory.size()*_resolution << " steps";
		throw Engine::Exception(oss.str());

	}
	return resourceHistory[index];
}

Engine::StaticRaster & SimulationRecord::getStaticRaster( const std::string & key )
{	
	StaticRasterMap::iterator it = _staticRasters.find(key);
	// the key does not exists
	if(it==_staticRasters.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getStaticRaster - searching for unknown key: " << key << " in static rasters";
		throw Engine::Exception(oss.str());
	}
	return it->second;
}

const std::string & SimulationRecord::getName()
{
	return _name;
}

int SimulationRecord::getNumSteps()
{
	return _numSteps;
}

int SimulationRecord::getResolution()
{
	return _resolution;
}

SimulationRecord::AgentTypesMap::iterator SimulationRecord::beginTypes()
{
	return _types.begin();
}

SimulationRecord::AgentTypesMap::iterator SimulationRecord::endTypes()
{
	return _types.end();
}

SimulationRecord::AgentRecordsMap::iterator SimulationRecord::beginAgents( const std::string & type )
{
	AgentTypesMap::iterator it = _types.find(type);
	if(it==_types.end())
	{	
		std::stringstream oss;
		oss << "SimulationRecord::beginAgents - asking for type " << type;
		throw Engine::Exception(oss.str());
	}	
	return it->second.begin();
}

SimulationRecord::AgentRecordsMap::iterator SimulationRecord::endAgents( const std::string & type )
{
	AgentTypesMap::iterator it = _types.find(type);
	if(it==_types.end())
	{	
		std::stringstream oss;
		oss << "SimulationRecord::endAgents- asking for type " << type;
		throw Engine::Exception(oss.str());
	}	
	return it->second.end();
}

SimulationRecord::AgentRecordsMap::iterator SimulationRecord::beginAgents( AgentTypesMap::iterator & it )
{
	return it->second.begin();
}

SimulationRecord::AgentRecordsMap::iterator SimulationRecord::endAgents( AgentTypesMap::iterator & it )
{
	return it->second.end();
}

SimulationRecord::RasterMap::iterator SimulationRecord::beginRasters()	
{
	return _resources.begin();
}

SimulationRecord::RasterMap::iterator SimulationRecord::endRasters()
{
	return _resources.end();
}

SimulationRecord::StaticRasterMap::iterator SimulationRecord::beginStaticRasters()	
{
	return _staticRasters.begin();
}

SimulationRecord::StaticRasterMap::iterator SimulationRecord::endStaticRasters()
{
	return _staticRasters.end();
}
		
AgentRecord * SimulationRecord::getAgentAtPosition( int step, const Engine::Point2D<int> & position )
{
	for(AgentTypesMap::iterator itType=_types.begin(); itType!=_types.end(); itType++)
	{
		for(AgentRecordsMap::iterator it=beginAgents(itType); it!=endAgents(itType); it++)
		{
			AgentRecord * agentRecord = it->second;	
			int x = agentRecord->getState(step, "x");
			int y = agentRecord->getState(step, "y");
			if(x==position._x && y==position._y)
			{
				return agentRecord;
			}
		}
	}
	return 0;
}

double SimulationRecord::getMean( const std::string & type, const std::string & state, int step )
{
	double value = 0;
	int sample = 0;
	AgentTypesMap::iterator itType = _types.find(type);
	if(itType==_types.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getMean - asking for state: " << state << " in type " << type;
		throw Engine::Exception(oss.str());
	}
	AgentRecordsMap agents = itType->second;
	for(AgentRecordsMap::iterator it=agents.begin(); it!=agents.end(); it++)
	{
		AgentRecord * agentRecord = it->second;	
		bool exists = agentRecord->getState(step, "exists");
		if(exists)
		{
			int agentValue = agentRecord->getState(step, state);
			if(agentValue!=std::numeric_limits<int>::max())
			{
				value += agentValue;
				sample++;
			}
		}
	}
	value = value/sample;
	return value;
}

double SimulationRecord::getSum( const std::string & type, const std::string & state, int step )
{
	double value = 0;
	
	AgentTypesMap::iterator itType = _types.find(type);
	if(itType==_types.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getSum - asking for state: " << state << " in type " << type;
		throw Engine::Exception(oss.str());
	}
	AgentRecordsMap agents = itType->second;
	for(AgentRecordsMap::iterator it=agents.begin(); it!=agents.end(); it++)
	{
		AgentRecord * agentRecord = it->second;	
		bool exists = agentRecord->getState(step, "exists");
		if(exists)
		{
			int agentValue = agentRecord->getState(step, state);	
			if(agentValue!=std::numeric_limits<int>::max())
			{
				value += agentValue;
			}
		}
	}
	return value;
}

void SimulationRecord::registerAgent( hid_t loc_id, const char * name )
{
	hid_t datasetId = H5Dopen(loc_id, name, H5P_DEFAULT);
	std::string agentName(name);

	unsigned int typePos = agentName.find_first_of("_");
	std::string type = agentName.substr(0,typePos);

	AgentTypesMap::iterator typeIt = _types.find(type);
	if(typeIt==_types.end())
	{
		_types.insert( make_pair( type, AgentRecordsMap()));
		typeIt = _types.find(type);
	}
	AgentRecordsMap & agents = typeIt->second;
	AgentRecordsMap::iterator it = agents.find(agentName);
	// new agent
	if(it==agents.end())
	{
		// +1 due to init state, new agent record with size of num steps +1
		agents.insert( make_pair( agentName, new AgentRecord(agentName, (_numSteps/_resolution)+1)));
		it = agents.find(agentName);
	}

	int numAttributes = H5Aget_num_attrs(datasetId);
	for(int i=0; i<numAttributes; i++)
	{
		char nameAttribute[256];
		hid_t attributeId= H5Aopen_idx(datasetId, i);
		H5Aget_name(attributeId, 256, nameAttribute);
		int value = 0;
		H5Aread(attributeId, H5T_NATIVE_INT, &value);

	
		AgentRecord * agentRecord = it->second;
		agentRecord->addState( _loadingStep/_resolution, std::string(nameAttribute), value );
		H5Aclose(attributeId);

		ValuesMap::iterator it = _minAttributeValues.find(nameAttribute);
		if(it==_minAttributeValues.end())
		{
			_minAttributeValues.insert( make_pair(std::string(nameAttribute), value) );
		}
		else if(value<it->second)
		{
			it->second = value;
		}

		it = _maxAttributeValues.find(nameAttribute);
		if(it==_maxAttributeValues.end())
		{
			_maxAttributeValues.insert( make_pair(std::string(nameAttribute), value) );
		}
		else if(value>it->second)
		{
			it->second = value;
		}
	}
	H5Dclose(datasetId);
}

int SimulationRecord::getMinValueForState( const std::string & state )
{
	ValuesMap::iterator it = _minAttributeValues.find(state);
	if(it==_minAttributeValues.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getMinValueForState - asking for not registered state: " << state;
		throw Engine::Exception(oss.str());
	}
	return it->second;
}

int SimulationRecord::getMaxValueForState( const std::string & state )
{	
	ValuesMap::iterator it = _maxAttributeValues.find(state);
	if(it==_maxAttributeValues.end())
	{
		std::stringstream oss;
		oss << "SimulationRecord::getMaxValueForState - asking for not registered state: " << state;
		throw Engine::Exception(oss.str());
	}
	return it->second;

}

herr_t SimulationRecord::registerAgentStep( hid_t loc_id, const char *name, void *opdata )
{
	Engine::SimulationRecord * record = (Engine::SimulationRecord*)opdata;
	record->registerAgent(loc_id, name);
    return 0;
}

int SimulationRecord::getSize()
{
	return _size;
}

const float & SimulationRecord::getLoadingPercentageDone() const
{
	return _loadingPercentageDone;
}

const std::string & SimulationRecord::getLoadingState() const
{
	return _loadingState;
}

} // namespace Engine


