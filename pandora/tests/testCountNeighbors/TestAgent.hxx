
#ifndef __TestAgent_hxx__
#define __TestAgent_hxx__

#include "Agent.hxx"
#include <string>

#include "Point2D.hxx"

namespace Engine
{
	class Agent;
}

namespace Test
{

typedef struct
{
	char _id[100];	
	Engine::Point2D<int> _position;
} TestAgentPackage;

class TestAgent: public Engine::Agent
{	

public:
	TestAgent( const std::string & id );
	TestAgent( const TestAgentPackage & package );
	virtual ~TestAgent();
	
	void serialize();
	void step();
	void * createPackage();
};

} // namespace Test

#endif // __TestAgent_hxx__

