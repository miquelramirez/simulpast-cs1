
#include "TestAgentA.hxx"
#include "World.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include <cstring>

namespace Test
{

TestAgentA::TestAgentA( const std::string & id ) : Agent(id), _testValueA(10)
{
	_testId = "test agent A";
}

TestAgentA::TestAgentA( const TestAgentAPackage & package ) : Agent(package._id)
{
	_position = package._position;
	_testValueA = package._testValueA;
	_testId = package._testId;		
}

TestAgentA::~TestAgentA()
{
}

void TestAgentA::move()
{
	Engine::Position<int> newPosition = _position;
	newPosition._x++;

	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
		return;
	}
}

void TestAgentA::step()
{
	move();
}

void * TestAgentA::createPackage()
{
	TestAgentAPackage * package = new TestAgentAPackage;	
	memcpy(&package->_id, _id.c_str(), sizeof(char)*_id.size());
	package->_id[_id.size()] = '\0';
	package->_position = _position;
	package->_testValueA= _testValueA;
	
	memcpy(&package->_testId, _testId.c_str(), sizeof(char)*_testId.size());
	package->_testId[_testId.size()] = '\0';

	return package;
}

void TestAgentA::store()
{
	storeAttribute("testValueA", _testValueA);
	//storeAttribute("testId", _testId);
}
	
const std::string & TestAgentA::getTestId()
{
	return _testId;
}

const int & TestAgentA::getTestValueA()
{
	return _testValueA;
}

} // namespace Test

