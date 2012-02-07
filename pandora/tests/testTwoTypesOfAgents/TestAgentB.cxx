
#include "TestAgentB.hxx"
#include "World.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include <cstring>

namespace Test
{

TestAgentB::TestAgentB( const std::string & id ) : Agent(id), _testValueB(5)
{
}

TestAgentB::TestAgentB( const TestAgentBPackage & package ) : Agent(package._id)
{
	_position = package._position;
	_testValueB = package._testValueB;
}

TestAgentB::~TestAgentB()
{
}

void TestAgentB::move()
{
	Engine::Position<int> newPosition = _position;
	newPosition._x++; // += 2;

	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
		return;
	}
}

void TestAgentB::step()
{
	move();
}

void * TestAgentB::createPackage()
{
	TestAgentBPackage * package = new TestAgentBPackage;	
	memcpy(&package->_id, _id.c_str(), sizeof(char)*_id.size());
	package->_id[_id.size()] = '\0';
	package->_position = _position;
	package->_testValueB = _testValueB;	
	
	return package;
}

void TestAgentB::store()
{
	storeAttribute("testValueB", _testValueB);
}

const int & TestAgentB::getTestValueB()
{
	return _testValueB;
}
} // namespace Test

