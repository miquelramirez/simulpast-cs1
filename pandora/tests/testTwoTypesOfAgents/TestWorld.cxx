
#include "TestWorld.hxx"

#include "Raster.hxx"
#include "Position.hxx"
#include "Exceptions.hxx"

#include "Agent.hxx"
#include "TestAgentA.hxx"
#include "TestAgentB.hxx"

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
	if(_id==2 || _id==3)
	{
		return;
	}
	if(_numTasks==1)
	{
		assert(_agents.size()==2);
	}
	else if(_numTasks==4)
	{
		if(_id==0)
		{
			if(_step<32)
			{
				assert(_agents.size()==2);
				assert(_overlapAgents.size()==0);
			}
			else if(_step<36)
			{
				assert(_agents.size()==0);
				assert(_overlapAgents.size()==2);
			}
			else
			{
				assert(_agents.size()==0);
				assert(_overlapAgents.size()==0);
			}
		}
		else if(_id==1)
		{
			if(_step<28)				
			{
				assert(_agents.size()==0);
				assert(_overlapAgents.size()==0);
			}
			else if(_step<32)
			{
				assert(_agents.size()==0);
				assert(_overlapAgents.size()==2);
			}
			else
			{
				assert(_agents.size()==2);
				assert(_overlapAgents.size()==0);
			}
		}
	}
	if(_agents.size()!=0)
	{
		TestAgentA * agentA = (TestAgentA*)getAgent("testA_1");
		assert(agentA->isType("testA")==true);
		assert(agentA->isType("testB")==false);
		assert(agentA->getTestValueA()==10);
		assert(agentA->getTestId().compare("test agent A")==0);

		TestAgentB * agentB = (TestAgentB*)getAgent("testB_1");
		assert(agentB->isType("testA")==false);
		assert(agentB->isType("testB")==true);	
		assert(agentB->getTestValueB()==5);
	}
}

void TestWorld::createAgents()
{
	if(_id==0)
	{
		TestAgentA * agent = new TestAgentA("testA_1");
		agent->setPosition(Engine::Position<int>(0,10));
		addAgent(agent);
		
		TestAgentB * agent2 = new TestAgentB("testB_1");
		agent2->setPosition(Engine::Position<int>(0,20));
		addAgent(agent2);
		return;
	}
}

Engine::Agent * TestWorld::createAgentFromPackage( const std::string & type, void * package )
{
	if(type.compare("testA")==0)
	{
		TestAgentAPackage * formattedPackage = 0;
		try
		{
			formattedPackage = (TestAgentAPackage *)package;
		}
		catch(std::exception & e)
		{
			std::cout << "TestWorld::createAgentFromPackage, wrong cast with type: " << type << " and exception: " << e.what() << std::endl;
		}
		return (Engine::Agent*)(new TestAgentA(*formattedPackage));
	}
	else if(type.compare("testB")==0)
	{
		TestAgentBPackage * formattedPackage = 0;
		try
		{
			formattedPackage = (TestAgentBPackage *)package;
		}
		catch(std::exception & e)
		{
			std::cout << "TestWorld::createAgentFromPackage, wrong cast with type: " << type << " and exception: " << e.what() << std::endl;
		}
		return (Engine::Agent*)(new TestAgentB(*formattedPackage));
	}
	else
	{	
		std::stringstream oss;
		oss << "TestWorld::createAgentFromPackage - unknown agent type: " << type;
		throw Engine::Exception(oss.str());
	}
}

void * TestWorld::createPackage( const std::string & type )
{
	if(type.compare("testA")==0)
	{
		return new TestAgentAPackage;
	}
	else if(type.compare("testB")==0)
	{
		return new TestAgentBPackage;
	}
	else
	{	
		std::stringstream oss;
		oss << "TestWorld::createPackage - unknown agent type: " << type;
		throw Engine::Exception(oss.str());
	}
}

void TestWorld::registerTypes()
{
	_types.insert( std::make_pair( "testA", createTypeA()));
	_types.insert( std::make_pair( "testB", createTypeB()));
}

MPI_Datatype * TestWorld::createTypeA()
{
	TestAgentAPackage package;
	// we will send id, x and y
	int blockLengths[5];

	// 100 chars
	blockLengths[0] = blockLengths[4] = 100;
	blockLengths[1] = blockLengths[2]  = blockLengths[3] = 1;

	// typelist
	MPI_Datatype typelist[5];
	typelist[0] = typelist[4] = MPI_CHAR;
	typelist[1] = typelist[2] = typelist[3] = MPI_INT;

	MPI_Aint displacements[5];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;
	MPI_Address(&package._testValueA, &address);
	displacements[3] = address-startAddress;
	MPI_Address(&package._testId, &address);
	displacements[4] = address-startAddress;

	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(5, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
	return newDataType;
}

MPI_Datatype * TestWorld::createTypeB()
{
	TestAgentBPackage package;
	// we will send id, x and y
	int blockLengths[4];

	// 100 chars
	blockLengths[0] = 100;
	blockLengths[1] = blockLengths[2]  = blockLengths[3] = 1;

	// typelist
	MPI_Datatype typelist[4];
	typelist[0] = MPI_CHAR;
	typelist[1] = typelist[2] = typelist[3] = MPI_INT;

	MPI_Aint displacements[4];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;
	MPI_Address(&package._testValueB, &address);
	displacements[3] = address-startAddress;

	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(4, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
	return newDataType;
}

} // namespace Test 

