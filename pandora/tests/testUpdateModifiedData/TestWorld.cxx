
#include "TestWorld.hxx"

#include "Raster.hxx"
#include "Position.hxx"
#include "Exceptions.hxx"

#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld() : World(4, 64, 4, true, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::createRasters()
{	
	registerRaster("test");
	getRaster("test").setInitValues(0,2);
}

void TestWorld::stepRaster( const std::string & key, Engine::Raster & raster )
{
	/*
	if(_step<5)
	{
		for(int i=0; i<_step; i++)
		{
			Engine::Position<int> pos(i,i);
			if(_overlapBoundaries.isInside(pos))
			{
				assert(getValue("test", pos)==std::min(2,_step));
			}
		}
	}
	else*/
	if(_step==5)
	{	
		Engine::Position<int> index;
		for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
		{
			for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
			{
				setMaxValue("test", index, 4);
			}
		}
	}
}

void TestWorld::createAgents()
{
}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & key, void * package )
{
	return 0;
}

void * TestWorld::createPackage( const std::string & key )
{
	return 0;
}

void TestWorld::registerTypes()
{
}

} // namespace Test 

