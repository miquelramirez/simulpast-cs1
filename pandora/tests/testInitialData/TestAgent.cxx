
#include "TestAgent.hxx"
#include "World.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include <cstring>

namespace Test
{

TestAgent::TestAgent( const std::string & id ) : Agent(id)
{
}

TestAgent::TestAgent( const TestAgentPackage & package ) : Agent(package._id)
{
	_position = package._position;
}

TestAgent::~TestAgent()
{
}

void TestAgent::move()
{
	Engine::Position<int> newPosition = _position;
	newPosition._x++;
	newPosition._y++;
	
	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
		_world->setValue("test", _position, _world->getCurrentStep()+1);
		return;
	}
}

void TestAgent::step()
{
	move();
}

void * TestAgent::createPackage()
{
	TestAgentPackage * package = new TestAgentPackage;	
	memcpy(&package->_id, _id.c_str(), sizeof(char)*_id.size());
	package->_id[_id.size()] = '\0';
	package->_position = _position;
	
	return package;
}

void TestAgent::store()
{
}

} // namespace Test

