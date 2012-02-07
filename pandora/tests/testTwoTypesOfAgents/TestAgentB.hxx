
#ifndef __TestAgentB_hxx__
#define __TestAgentB_hxx__

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
	int _testValueB;
} TestAgentBPackage;

class TestAgentB: public Engine::Agent
{
	int _testValueB;

	void move();
	void store();

public:
	TestAgentB( const std::string & id );
	TestAgentB( const TestAgentBPackage & package );
	virtual ~TestAgentB();
	
	void step();
	void * createPackage();
	
	const int & getTestValueB();
};

} // namespace Test

#endif // __TestAgentB_hxx__

