
#include "TestAgent.hxx"
#include "World.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include <cstring>

namespace Test
{

TestAgent::TestAgent( const std::string & id, const bool & horizontalMovement ) : Agent(id), _horizontalMovement(horizontalMovement)
{
}

TestAgent::TestAgent( const TestAgentPackage & package ) : Agent(package._id)
{
	_position = package._position;
	_horizontalMovement = package._horizontalMovement;
}

TestAgent::~TestAgent()
{
}

void TestAgent::move()
{
	Engine::Position<int> newPosition = _position;
	if(_horizontalMovement)
	{
		newPosition._x++;
	}
	else
	{
		newPosition._y++;
	}
	//std::cout << "agent : " << this << " trying to move to: " << newPosition << std::endl;

	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
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
	package->_horizontalMovement = _horizontalMovement;
	
	return package;
}

void TestAgent::store()
{
}

} // namespace Test

