
#include "TestWorld.hxx"

#include "Raster.hxx"
#include "Position.hxx"
#include "Exceptions.hxx"

#include "Agent.hxx"
#include "TestAgent.hxx"

#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld() : World(64, 64, 4, true, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::createRasters()
{	
	registerRaster("test");
	getRaster("test").setInitValues(0,0);
}

void TestWorld::stepAgents()
{
	if(_id==3)
	{
		if(_step<31)
		{	
			assert(_agents.size()==1);
			assert(_overlapAgents.size()==0);
		}
		else
		{
			assert(_agents.size()==0);
			assert(_overlapAgents.size()==0);
		}
		return;
	}
	if(_step<28)
	{	
		assert(_agents.size()==0);
		assert(_overlapAgents.size()==0);
	}
	else if(_step<31)
	{	
		assert(_agents.size()==0);
		assert(_overlapAgents.size()==1);
	}
	else
	{
		assert(_agents.size()==0);
		assert(_overlapAgents.size()==0);
	}
}

void TestWorld::createAgents()
{
	if(_id==3)
	{
		TestAgent * agent = new TestAgent("test_0");
		agent->setPosition(Engine::Position<int>(63,63));
		addAgent(agent);
		return;
	}
}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & key, void * package )
{
	TestAgentPackage * formattedPackage = 0;
	try
	{
		formattedPackage = (TestAgentPackage *)package;
	}
	catch(std::exception & e)
	{
		std::cout << "TestWorld::createAgentFromPackage, wrong cast with exception: " << e.what() << std::endl;
	}
	return (Engine::Agent*)(new TestAgent(*formattedPackage));
}

void * TestWorld::createPackage(const std::string & key)
{
	return new TestAgentPackage;
}

void TestWorld::registerTypes()
{
	TestAgentPackage package;
	// we will send id, x and y
	int blockLengths[3];

	// 100 chars
	blockLengths[0] = 100;
	blockLengths[1] = blockLengths[2]  = 1;

	// typelist
	MPI_Datatype typelist[3];
	typelist[0] = MPI_CHAR;
	typelist[1] = typelist[2] = MPI_INT;

	MPI_Aint displacements[3];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;

	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(3, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
}

} // namespace Test 

