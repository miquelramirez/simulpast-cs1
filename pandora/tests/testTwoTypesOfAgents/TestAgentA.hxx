
#ifndef __TestAgentA_hxx__
#define __TestAgentA_hxx__

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
	int _testValueA;
	char _testId[100];
} TestAgentAPackage;

class TestAgentA: public Engine::Agent
{	
	int _testValueA;
	std::string _testId;

	void move();
	void store();

public:
	TestAgentA( const std::string & id );
	TestAgentA( const TestAgentAPackage & package );
	virtual ~TestAgentA();
	
	void step();
	void * createPackage();
	const std::string & getTestId();
	const int & getTestValueA();
};

} // namespace Test

#endif // __TestAgentA_hxx__

