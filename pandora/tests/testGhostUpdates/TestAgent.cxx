
#include "TestAgent.hxx"
#include "World.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include <cstring>

namespace Test
{

TestAgent::TestAgent( const std::string & id, const bool & horizontalMovement ) : Agent(id), _horizontalMovement(horizontalMovement), _evenTurn(true)
{
}

TestAgent::TestAgent( const TestAgentPackage & package ) : Agent(package._id)
{
	_position = package._position;
	_horizontalMovement = package._horizontalMovement;
	_evenTurn = package._evenTurn;
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
	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
		return;
	}
}

void TestAgent::updateTurnInformation()
{
	if(_world->getCurrentStep()%2==0)
	{
		//std::cout << "agent: " << this << " on even turn in step: " << _world->getCurrentStep() << std::endl;
		_evenTurn = true;
	}
	else
	{		
		//std::cout << "agent: " << this << " on odd turn in step: " << _world->getCurrentStep() << std::endl;
		Engine::World::AgentsList neighbors = _world->getAgentsNear(_position, 1, false);
		for(Engine::World::AgentsList::iterator it=neighbors.begin(); it!=neighbors.end(); it++)
		{
			TestAgent * agent = (TestAgent*)(*it);
			//std::cout << "agent: " << this << " is updating: " << agent << std::endl;
			agent->setEvenTurn(false);
		}
	}
}

void TestAgent::setEvenTurn( const bool & evenTurn )
{
	_evenTurn = evenTurn;
}

bool TestAgent::isEvenTurn()
{
	return _evenTurn;
}

void TestAgent::step()
{
	move();
	updateTurnInformation();
}

void * TestAgent::createPackage()
{
	TestAgentPackage * package = new TestAgentPackage;	
	memcpy(&package->_id, _id.c_str(), sizeof(char)*_id.size());
	package->_id[_id.size()] = '\0';
	package->_position = _position;
	package->_horizontalMovement = _horizontalMovement;
	package->_evenTurn = _evenTurn;
	
	return package;
}

void TestAgent::store()
{
	storeAttribute("evenTurn", (int)_evenTurn);
}

} // namespace Test

