
#include "StaticRaster.hxx"
#include "Exceptions.hxx"
#include "World.hxx"

#include <gdal_priv.h>
#include <hdf5.h>

namespace Engine
{

StaticRaster::StaticRaster() : _maxValue(std::numeric_limits<int>::min()), _minValue(std::numeric_limits<int>::max())								   
{
}

StaticRaster::~StaticRaster()
{
}

void StaticRaster::resize( const Point2D<int> & size )
{
	_values.resize(size._x);
	// TODO explore how to improve performance with STL algorithms
	// for_each(_valuesField.begin(),_valuesField.end(),resize(_matrixSize));
	for(int i=0; i<size._x; i++)
	{
		_values[i].resize(size._y);
	}
}

const int & StaticRaster::getValue( Point2D<int> position ) const
{
	if(position._x<0 || position._x>=_values.size())
	{
		std::stringstream oss;
		oss << "StaticRaster::getValue - " << position << " x out of bounds: " << _values.size();
		throw Exception(oss.str());
		return -1;
	}
	if(position._y<0 || position._y>=_values[position._x].size())
	{
		std::stringstream oss;
		oss << "StaticRaster::getValue - " << position << " y out of bounds: " << _values.size() << "/" << _values[position._x].size();
		throw Exception(oss.str());
		return -1;
	}
	return _values[position._x][position._y];
}

Point2D<int> StaticRaster::getSize() const
{
	if(_values.size()==0)
	{
		return Point2D<int>(0,0);
	}
	return Point2D<int>(_values.size(), _values[0].size());
}

void StaticRaster::loadGDALFile( const std::string & fileName, World & world )
{
	Simulation & simulation(world.getSimulation());
	GDALAllRegister();
	GDALDataset * dataset = (GDALDataset *)GDALOpen(fileName.c_str(), GA_ReadOnly );
	
	if(!dataset)
	{
		std::stringstream oss;
		oss << "StaticRaster::loadGDALFile - file: " << fileName << " not found";
		throw Engine::Exception(oss.str());
	}

	int size = dataset->GetRasterXSize();
	if(size!=dataset->GetRasterYSize())
	{
		std::stringstream oss;
		oss << "StaticRaster::loadFile - file: " << fileName << " does not contain an squared raster. width: " << size << " and height: " << dataset->GetRasterYSize();
		throw Engine::Exception(oss.str());
	}
	if(size!=simulation.getSize())
	{
		std::stringstream oss;
		oss << "StaticRaster::loadFile - file: " << fileName << " with size: " << size << " different from defined size: " << simulation.getSize() << std::endl;
		throw Engine::Exception(oss.str());
	}

	GDALRasterBand * band = dataset->GetRasterBand(1);
	double minMaxValues[2];
	int hasMin, hasMax;
	minMaxValues[0] = band->GetMinimum(&hasMin);
	minMaxValues[1] = band->GetMaximum(&hasMax);
	if(!(hasMin && hasMax))
	{
		GDALComputeRasterMinMax((GDALRasterBandH)band, TRUE, minMaxValues);
	}
	
	float * pafScanline = (float *)CPLMalloc(sizeof(float)*(simulation.getLocalRasterSize()*simulation.getLocalRasterSize()));
	int worldsPerRow = sqrt(simulation.getNumTasks());
	int column = simulation.getId()/worldsPerRow;
	int row = simulation.getId()%worldsPerRow;		
	band->RasterIO( GF_Read, row*simulation.getLocalRasterSize(), column*simulation.getLocalRasterSize(), simulation.getLocalRasterSize(), simulation.getLocalRasterSize(), pafScanline, simulation.getLocalRasterSize(), simulation.getLocalRasterSize(), GDT_Float32, 0, 0 );

	const Rectangle<int> & boundaries = world.getBoundaries();
	Point2D<int> index;
	for(index._x=boundaries._origin._x; index._x<boundaries._origin._x+boundaries._size._x; index._x++)
	{
		for(index._y=boundaries._origin._y; index._y<boundaries._origin._y+boundaries._size._y; index._y++)
		{
			Point2D<int> index2(index - boundaries._origin);
			int value = (int)(pafScanline[simulation.getLocalRasterSize()*index2._y+index2._x]);
			if(value>_maxValue)
			{
				_maxValue = value;
			}
			if(value<_minValue)
			{
				_minValue = value;
			}
			_values[index._x][index._y] = value;
		}
	}
	GDALClose(dataset);
}

void StaticRaster::loadHDF5File( const std::string & fileName, const std::string & rasterName )
{
	std::cout << "loading static raster " << rasterName << " from: " << fileName  << "...";
	std::ostringstream oss;
	oss << "/" << rasterName << "/values";	
	hid_t fileId = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	hid_t dset_id = H5Dopen(fileId, oss.str().c_str());
	hid_t dataspaceId = H5Dget_space(dset_id);
	hsize_t dims[2];
	H5Sget_simple_extent_dims(dataspaceId, dims, NULL);
	H5Sclose(dataspaceId);
	//std::cout << "dims: " << dims[0] << "/" << dims[1] << std::endl;
	int * dset_data = (int*)malloc(sizeof(int)*dims[0]*dims[1]);
				
	// squared
	resize(Point2D<int>(dims[0], dims[1]));
	
	H5Dread(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dset_data);
	H5Dclose(dset_id);

	for(int i=0; i<dims[0]; i++)
	{
		for(int j=0; j<dims[1]; j++)
		{
			int index = i+j*dims[0];
			int value = dset_data[index];	
			if(value>_maxValue)
			{
				_maxValue = value;
			}
			if(value<_minValue)
			{
				_minValue = value;
			}
			_values[i][j] = value;
		}
	}
	free(dset_data);
	std::cout << std::endl;
	H5Fclose(fileId);
}

const int & StaticRaster::getMinValue() const
{
	return _minValue;
}

const int & StaticRaster::getMaxValue() const
{
	return _maxValue;
}

} // namespace Engine

