
#ifndef __TestAgent_hxx__
#define __TestAgent_hxx__

#include "Agent.hxx"
#include <string>

#include "Position.hxx"

namespace Engine
{
	class Agent;
}

namespace Test
{

typedef struct
{
	char _id[100];	
	Engine::Position<int> _position;
} TestAgentPackage;

class TestAgent: public Engine::Agent
{	
	void move();
	void store();

public:
	TestAgent( const std::string & id );
	TestAgent( const TestAgentPackage & package );
	virtual ~TestAgent();
	
	void step();
	void * createPackage();
};

} // namespace Test

#endif // __TestAgent_hxx__

