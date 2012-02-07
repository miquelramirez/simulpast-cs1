
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
	bool _horizontalMovement;
	bool _evenTurn;
} TestAgentPackage;

class TestAgent: public Engine::Agent
{	
	void move();
	void store();

	bool _horizontalMovement;
	bool _evenTurn;

	void updateTurnInformation();
	void setEvenTurn( const bool & evenTurn );
public:
	TestAgent( const std::string & id, const bool & horizontalMovement );
	TestAgent( const TestAgentPackage & package );
	virtual ~TestAgent();
	
	bool isEvenTurn();
	void step();
	void * createPackage();
};

} // namespace Test

#endif // __TestAgent_hxx__

