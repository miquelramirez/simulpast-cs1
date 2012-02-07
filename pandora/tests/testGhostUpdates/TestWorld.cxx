
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
	// it is important to think that step agents is executed before agents, so each step must check the actions in the former step
	if(_step==0)
	{
		return;
	}
	int lastStep = _step - 1;
	bool lastStepWasEven = false;
	if(lastStep%2==0)
	{
		lastStepWasEven = true;
	}
	for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
	{
		TestAgent * agent = (TestAgent*)(*it);
		assert(agent->isEvenTurn()==lastStepWasEven);
	}
	for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)
	{
		TestAgent * agent = (TestAgent*)(*it);
		assert(agent->isEvenTurn()==lastStepWasEven);
	}
}

void TestWorld::createAgents()
{
	if(_id==0)
	{
		TestAgent * agentVertical1 = new TestAgent("test_0", false);
		Engine::Position<int> pos(31,0);
		agentVertical1->setPosition(pos+_boundaries._origin);
		addAgent(agentVertical1);
		TestAgent * agentHorizontal1 = new TestAgent("test_2", true);
		Engine::Position<int> pos2(0,31);
		agentHorizontal1->setPosition(pos2+_boundaries._origin);
		addAgent(agentHorizontal1);
		return;
	}
	if(_id==1)
	{
		TestAgent * agentVertical2 = new TestAgent("test_1", false);
		Engine::Position<int> pos(0,0);
		agentVertical2->setPosition(pos+_boundaries._origin);
		addAgent(agentVertical2);
		return;

	}
	if(_id==2)
	{
		TestAgent * agentHorizontal2 = new TestAgent("test_3", true);
		Engine::Position<int> pos(0,0);
		agentHorizontal2->setPosition(pos+_boundaries._origin);
		addAgent(agentHorizontal2);
		return;
	}
}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & type, void * package )
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

void * TestWorld::createPackage(const std::string & type)
{
	return new TestAgentPackage;
}

void TestWorld::registerTypes()
{
	TestAgentPackage package;
	// we will send id, x and y
	int blockLengths[5];

	// 100 chars
	blockLengths[0] = 100;
	blockLengths[1] = blockLengths[2] = blockLengths[3] = blockLengths[4] = 1;

	// typelist
	MPI_Datatype typelist[5];
	typelist[0] = MPI_CHAR;
	typelist[1] = typelist[2] = typelist[3] = typelist[4] = MPI_INT;

	MPI_Aint displacements[5];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;
	MPI_Address(&package._horizontalMovement, &address);
	displacements[3] = address-startAddress;
	MPI_Address(&package._evenTurn, &address);
	displacements[4] = address-startAddress;

	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(5, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
}

} // namespace Test 

