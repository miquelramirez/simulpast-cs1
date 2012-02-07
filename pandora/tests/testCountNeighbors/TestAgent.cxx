
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

void TestAgent::step()	
{
	for(int i=0; i<100; i++)
	{
		_world->countNeighbours(this, 10);
	}
}

void * TestAgent::createPackage()
{
	TestAgentPackage * package = new TestAgentPackage;	
	memcpy(&package->_id, _id.c_str(), sizeof(char)*_id.size());
	package->_id[_id.size()] = '\0';
	package->_position = _position;
	
	return package;
}

void TestAgent::serialize()
{
}

} // namespace Test

