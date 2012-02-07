
#include "Agent.hxx"
#include "Exceptions.hxx"
#include "World.hxx"
#include "Serializer.hxx"

#include <iostream>

namespace Engine
{

Agent::Agent( const std::string & id ) : _id(id), _exists(true), _position(-1,-1), _world(0)
{
}

Agent::~Agent()
{
}

void Agent::setWorld( World * world )
{
	_world = world;
}

World * Agent::getWorld()
{
	return _world;
}

void Agent::setPosition( const Point2D<int> & position )
{
	_position = position;
}

const Point2D<int> & Agent::getPosition()
{
	return _position;
}

void Agent::serializeAttribute( const std::string & name, const int & value )
{
	_world->getSerializer().serializeAttribute(name,value);
}

const std::string & Agent::getId()
{
	return _id;
}

bool Agent::exists()
{
	return _exists;
}

void Agent::setExists(bool exists)
{
	_exists = exists;
}

std::ostream & operator<<( std::ostream & stream, Agent * agent )
{
	if(agent->getWorld())
	{
		return stream << "id: " << agent->getId() << " pos: " << agent->getPosition() << " exists: " << agent->exists() << " at world: " << agent->getWorld()->getId();
	}
	else
	{
		return stream << "id: " << agent->getId() << " pos: " << agent->getPosition() << " exists: " << agent->exists() << " without world";
	}
}
	
void Agent::remove()
{
	_exists = false;
	if(!_world)
	{
		std::stringstream oss;
		oss << "Agent::remove - removing agent without assigned World";
		throw Exception(oss.str());
	}
	_world->removeAgent(this);
}
	
bool Agent::isType( const std::string & type )
{
	if(type.compare(getType())==0)
	{
		return true;
	}
	return false;
}

std::string Agent::getType()
{
	unsigned int typePos = _id.find_first_of("_");
	return _id.substr(0,typePos);
}

/*
void Agent::interactionWithAgent( Agent * agent )
{
	_environment->agentModified(agent);
}
*/

} // namespace Engine

