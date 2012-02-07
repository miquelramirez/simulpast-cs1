
#include "TestWorld.hxx"

#include "Exceptions.hxx"
#include "TestAgent.hxx"
#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld( Engine::Simulation & simulation ) : World( simulation, 1, false, "data/test.h5")
{
	setSearchAgents(true);
}

TestWorld::~TestWorld()
{
}

void TestWorld::stepAgents()
{

	if(_simulation.getNumTasks()==1)
	{
		Engine::Agent * agent = getAgent("test_0");
		int euclideanDistance = sqrt(6*6+6*6);

		assert(countNeighbours(agent, 7, "test")==0);
		assert(countNeighbours(agent, 8, "test")==1);
		assert(countNeighbours(agent, euclideanDistance, "test")==1);
		assert(countNeighbours(agent, 9, "test")==1);

		Engine::World::AgentsList neighbors = getNeighbours(agent, euclideanDistance);
		Engine::Agent * neighbor = *neighbors.begin();
		assert(neighbor->getId().compare("test_1")==0);
		return;
	}
	if(_simulation.getId()==0)
	{
		Engine::Agent * agent = getAgent("test_0");
		std::cout << "dist 5: " << countNeighbours(agent,5) << " dist 6: " << countNeighbours(agent,6) << " dist 7: " << countNeighbours(agent,7) << std::endl;
		assert(countNeighbours(agent,5)==0);
		assert(countNeighbours(agent,6)==3);
		assert(countNeighbours(agent,7)==3);
		assert(countNeighbours(agent,1000)==3);
	}
	
}

void TestWorld::createRasters()
{
	registerRaster("test");
}

void TestWorld::createAgents()
{
	if(_simulation.getId()==0)
	{
		TestAgent * agent = new TestAgent("test_0");
		agent->setPosition(Engine::Point2D<int>(31,31));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_1");
		agent2->setPosition(Engine::Point2D<int>(25,25));
		addAgent(agent2);
		
		TestAgent * agent3 = new TestAgent("test_3");
		agent3->setPosition(Engine::Point2D<int>(0,0));
		addAgent(agent3);

		for(int i=4; i<500; i++)
		{	
			std::ostringstream oss;
			oss << "foo_" << i;
			TestAgent * agentN = new TestAgent(oss.str());
			bool located = false;
			while(!located)
			{
				Engine::Point2D<int> pos(_statistics.getUniformDistValue(0,_boundaries._size._x), _statistics.getUniformDistValue(0,_boundaries._size._y));
				pos += _boundaries._origin;
				if(checkPosition(pos) && _boundaries.isInside(pos))
				{
					agentN->setPosition(pos);
					located = true;
				}
			}
			addAgent(agentN);
		}
		return;
	}
	if(_simulation.getId()==1)
	{
		TestAgent * agent = new TestAgent("test_3");
		agent->setPosition(Engine::Point2D<int>(37,25));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_4");
		agent2->setPosition(Engine::Point2D<int>(63,0));
		addAgent(agent2);

		return;
	}
	if(_simulation.getId()==3)
	{
		TestAgent * agent = new TestAgent("test_5");
		agent->setPosition(Engine::Point2D<int>(37,37));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_6");
		agent2->setPosition(Engine::Point2D<int>(63,63));
		addAgent(agent2);

		return;
	}


}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & key, void * package )
{
	assert(key.compare("test")==0);
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

