
#include "TestWorld.hxx"

#include "Raster.hxx"
#include "Position.hxx"
#include "Exceptions.hxx"

#include "Agent.hxx"

#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld() : World(10, 64, 4, true, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::stepAgents()
{
	Engine::Position<int> index(0,0);
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
		{
			assert(getValue("testX", index)==index._x);
			assert(getValue("testY", index)==index._y);
		}
	}
}

void TestWorld::createRasters()
{	
	registerRaster("testX");
	registerRaster("testY");
	getRaster("testX").setInitValues(0,_globalBoundaries._size._x);
	getRaster("testY").setInitValues(0,_globalBoundaries._size._y);

	Engine::Position<int> index(0,0);
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
		{
			//std::cout << _id << " setting value: " << index << " with boundaries: " << _boundaries << " at pos: " << index-_overlapBoundaries._origin << std::endl;
			setMaxValue("testX", index, index._x);
			setMaxValue("testY", index, index._y);
		}
	}
	updateRasterToMaxValues("testX");
	updateRasterToMaxValues("testY");
}

void TestWorld::createAgents()
{
}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & key, void * package )
{
	return 0;
}

void * TestWorld::createPackage(const std::string & key)
{
	return 0;
}

void TestWorld::registerTypes()
{
}

} // namespace Test 

