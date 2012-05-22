
/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "Serializer.hxx"

#include "World.hxx"
#include "StaticRaster.hxx"
#include "Raster.hxx"
#include "Agent.hxx"
#include "Simulation.hxx"
#include "Exceptions.hxx"
#include <boost/filesystem.hpp>

namespace Engine
{

Serializer::Serializer( const std::string & fileName ) : _fileName(fileName), _agentsFileId(-1), _fileId(-1), _currentAgentDatasetId(-1)
{
}

Serializer::~Serializer()
{
}

void Serializer::init( Simulation & simulation, StaticRastersMap & staticRasters, RastersMap & rasters, World & world )
{
	// check if directory exists
	unsigned int filePos = _fileName.find_last_of("/");
	std::string path = _fileName.substr(0,filePos+1);

	// create dir where logs will be stored if it is not already created
	boost::filesystem::create_directory(path);

	// creating base file in a parallel environment
	hid_t propertyListId = H5Pcreate(H5P_FILE_ACCESS);

	// workaround, it crashes in serial without this clause
	if(simulation.getNumTasks()>1)		
	{
		H5Pset_fapl_mpio(propertyListId, MPI_COMM_WORLD, MPI_INFO_NULL);
	}
	_fileId = H5Fcreate(_fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, propertyListId);
	H5Pclose(propertyListId);

	// adding a group with global generic data
	hsize_t simpleDimension = 1;
	hid_t globalFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	hid_t globalDatasetId = H5Dcreate(_fileId, "global", H5T_NATIVE_INT, globalFileSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	hid_t attributeFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	hid_t attributeId = H5Acreate(globalDatasetId, "numSteps", H5T_NATIVE_INT, attributeFileSpace, H5P_DEFAULT, H5P_DEFAULT);
	H5Awrite(attributeId, H5T_NATIVE_INT, &simulation.getNumSteps());
	H5Sclose(attributeFileSpace);
	H5Aclose(attributeId);

	attributeFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	attributeId = H5Acreate(globalDatasetId, "size", H5T_NATIVE_INT, attributeFileSpace, H5P_DEFAULT, H5P_DEFAULT);
	H5Awrite(attributeId, H5T_NATIVE_INT, &simulation.getSize());
	H5Sclose(attributeFileSpace);
	H5Aclose(attributeId);

	attributeFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	attributeId= H5Acreate(globalDatasetId, "numTasks", H5T_NATIVE_INT, attributeFileSpace, H5P_DEFAULT, H5P_DEFAULT);
	H5Awrite(attributeId, H5T_NATIVE_INT, &simulation.getNumTasks());
	H5Sclose(attributeFileSpace);
	H5Aclose(attributeId);

	std::cout << " id: " << simulation.getId() << " size: " << simulation.getSize() << " num tasks: " << simulation.getNumTasks() << " and steps: " << simulation.getNumSteps() << std::endl;

	// we store the name of the rasters
	hid_t rasterNameFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	hid_t rasterNameDatasetId = H5Dcreate(_fileId, "rasters", H5T_NATIVE_INT, rasterNameFileSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	for(RastersMap::iterator it=rasters.begin(); it!=rasters.end(); it++)
	{
		if(world.rasterToSerialize(it->first))
		{
			attributeFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
			const std::string & name(it->first);
			attributeId = H5Acreate(rasterNameDatasetId, name.c_str(), H5T_NATIVE_INT, attributeFileSpace, H5P_DEFAULT, H5P_DEFAULT);
			H5Sclose(attributeFileSpace);
			H5Aclose(attributeId);
		}
	}
	H5Dclose(rasterNameDatasetId);

	rasterNameDatasetId = H5Dcreate(_fileId, "staticRasters", H5T_NATIVE_INT, rasterNameFileSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	for(StaticRastersMap::iterator it=staticRasters.begin(); it!=staticRasters.end(); it++)
	{
		if(world.rasterToSerialize(it->first))
		{
			attributeFileSpace = H5Screate_simple(1, &simpleDimension, NULL);
			const std::string & name(it->first);
			attributeId = H5Acreate(rasterNameDatasetId, name.c_str(), H5T_NATIVE_INT, attributeFileSpace, H5P_DEFAULT, H5P_DEFAULT);
			H5Sclose(attributeFileSpace);
			H5Aclose(attributeId);
		}
	}
	H5Dclose(rasterNameDatasetId);
	H5Sclose(rasterNameFileSpace);

	H5Dclose(globalDatasetId);
	H5Sclose(globalFileSpace);
	
	// creating a file with the agents of each computer node
	std::ostringstream oss;
	oss << path << "/agents-" << simulation.getId() << ".abm";
	_agentsFileId = H5Fcreate(oss.str().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	for(int i=0; i<=simulation.getNumSteps(); i++)
	{
		std::ostringstream oss;
		oss << "step" << i;
		hid_t stepGroupId  = H5Gcreate(_agentsFileId, oss.str().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		H5Gclose(stepGroupId);
	}
	
	int localRasterSize = simulation.getSize()/sqrt(simulation.getNumTasks());
	
	//the real size of the matrix is sqrt(num simulator)*matrixsize	
	hsize_t dimensions[2];
	dimensions[0] = hsize_t(simulation.getSize());
	dimensions[1] = hsize_t(simulation.getSize());

	// we need to specify the size where each computer node will be writing
	hsize_t chunkDimensions[2];
	chunkDimensions[0] = localRasterSize/2;
	chunkDimensions[0] += 2*world.getOverlap();
	chunkDimensions[1] = localRasterSize/2;
	chunkDimensions[1] += 2*world.getOverlap();
	
	propertyListId = H5Pcreate(H5P_DATASET_CREATE);
	H5Pset_chunk(propertyListId, 2, chunkDimensions);

	// static rasters
	for(StaticRastersMap::iterator it=staticRasters.begin(); it!=staticRasters.end(); it++)
	{
		if(world.rasterToSerialize(it->first))
		{
			// TODO 0 o H5P_DEFAULT??
			hid_t rasterGroupId = H5Gcreate(_fileId, it->first.c_str(), 0, H5P_DEFAULT, H5P_DEFAULT);
			hid_t fileSpace = H5Screate_simple(2, dimensions, NULL); 
			hid_t datasetId = H5Dcreate(rasterGroupId, "values", H5T_NATIVE_INT, fileSpace, H5P_DEFAULT, propertyListId, H5P_DEFAULT);
			H5Dclose(datasetId);
			H5Sclose(fileSpace);
			H5Gclose(rasterGroupId);
		}
	}	
	// dynamic rasters
	for(RastersMap::iterator it=rasters.begin(); it!=rasters.end(); it++)
	{
		if(world.rasterToSerialize(it->first))
		{
			// TODO 0 o H5P_DEFAULT??
			hid_t rasterGroupId = H5Gcreate(_fileId, it->first.c_str(), 0, H5P_DEFAULT, H5P_DEFAULT);
			for(int i=0; i<=simulation.getNumSteps(); i++)
			{  
				std::ostringstream oss;
				oss << "step" << i;
				hid_t stepFileSpace = H5Screate_simple(2, dimensions, NULL); 
				hid_t stepDatasetId = H5Dcreate(rasterGroupId, oss.str().c_str(), H5T_NATIVE_INT, stepFileSpace, H5P_DEFAULT, propertyListId, H5P_DEFAULT);
				H5Dclose(stepDatasetId);
				H5Sclose(stepFileSpace);
			}	
			H5Gclose(rasterGroupId);
		}
	}
	H5Pclose(propertyListId);
}

void Serializer::finish()
{
	H5Fclose(_fileId);
	H5Fclose(_agentsFileId);
}
	
void Serializer::serializeAgent( Agent * agent, const int & step )
{
	hsize_t simpleDimension = 1;
	hid_t fileSpace = H5Screate_simple(1, &simpleDimension, NULL);	
	
	std::ostringstream ossStep;
	ossStep << "step" << step << "/" << agent->getId();
	_currentAgentDatasetId = H5Dcreate(_agentsFileId, ossStep.str().c_str(), H5T_NATIVE_INT, fileSpace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// basic attributes of an agent
	serializeAttribute("x",agent->getPosition()._x);
	serializeAttribute("y",agent->getPosition()._y);
	serializeAttribute("exists",(int)agent->exists());
	
	agent->serialize();
	
	H5Sclose(fileSpace);
	H5Dclose(_currentAgentDatasetId);
	_currentAgentDatasetId = -1;
}

void Serializer::serializeAttribute( const std::string & name, const int & value )
{
	if(_currentAgentDatasetId==-1)
	{
		std::stringstream oss;
		oss << "Serializer::serializeAttribute - trying to serialize agent state while _currentAgentDatasetId is not initialized.";
		throw Exception(oss.str());
		return;
	}
	hsize_t simpleDimension = 1;
	hid_t fileSpace = H5Screate_simple(1, &simpleDimension, NULL);
	hid_t attributeId = H5Acreate(_currentAgentDatasetId, name.c_str(), H5T_NATIVE_INT, fileSpace, H5P_DEFAULT, H5P_DEFAULT);
	H5Awrite(attributeId, H5T_NATIVE_INT, &value);

	H5Aclose(attributeId);
	H5Sclose(fileSpace);
}

void Serializer::serializeRaster( const std::string & key, Raster & raster, World & world, const int & step )
{
	std::ostringstream oss;
	oss << "/" << key << "/step" << step;
	serializeRaster(raster,world, oss.str());
}

void Serializer::serializeStaticRaster( const std::string & key, StaticRaster & raster, World & world )
{
	std::ostringstream oss;
	oss << "/" << key << "/values";
	serializeRaster(raster, world, oss.str());
}

void Serializer::serializeRaster( StaticRaster & raster, World & world, const std::string & datasetKey )
{
	int posInsideMatrix[2];
	// initial pos in environment where data will be copied
	posInsideMatrix[0] = 0;
	posInsideMatrix[1] = 0;

	// if it is not a border, it will copy from overlap
	hsize_t	offset[2];
    offset[0] = world.getBoundaries()._origin._y;
    offset[1] = world.getBoundaries()._origin._x;
 
	hsize_t	block[2];
	block[0] = world.getBoundaries()._size._y;
	block[1] = world.getBoundaries()._size._x;


	hid_t dataSetId = H5Dopen(_fileId, datasetKey.c_str(), H5P_DEFAULT);
	hid_t fileSpace = H5Dget_space(dataSetId);
	
	hsize_t	stride[2];
	stride[0] = 1;
	stride[1] = 1;
	
	hsize_t count[2];
	count[0] = 1;
	count[1] = 1;
	
	H5Sselect_hyperslab(fileSpace, H5S_SELECT_SET, offset, stride, count, block);
 
	int * data = (int *) malloc(sizeof(int)*block[0]*block[1]);
	
	Point2D<int> overlapDist = world.getBoundaries()._origin-world.getOverlapBoundaries()._origin;
	for(int i=0; i<block[0]; i++)
	{
		for(int j=0; j<block[1]; j++)
		{	
			int index = i*block[1]+j;
			data[index] = raster.getValue(Point2D<int> (j+overlapDist._x,i+overlapDist._y));
		}
	}
    // Create property list for collective dataset write.
	hid_t propertyListId = H5Pcreate(H5P_DATASET_XFER);
	H5Pset_dxpl_mpio(propertyListId, H5FD_MPIO_INDEPENDENT);
    hid_t memorySpace = H5Screate_simple(2, block, NULL);
	H5Dwrite(dataSetId, H5T_NATIVE_INT, memorySpace, fileSpace, propertyListId, data);

	free(data);
	H5Pclose(propertyListId);
    H5Sclose(memorySpace);	
	H5Sclose(fileSpace);
	H5Dclose(dataSetId);
}
	
} // namespace Engine

