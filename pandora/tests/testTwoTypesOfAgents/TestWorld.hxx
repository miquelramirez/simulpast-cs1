
#ifndef __TestWorld_hxx__
#define __TestWorld_hxx__

#include "World.hxx"

namespace Test
{
class Raster;
class Position;

class TestWorld : public Engine::World
{
	void createRasters();
	MPI_Datatype * createTypeA();
	MPI_Datatype * createTypeB();
	void * createPackage( const std::string & type );
	void createAgents();
	Engine::Agent * createAgentFromPackage( const std::string & type, void * package );
	void stepAgents();
	void registerTypes();
public:
	TestWorld();
	virtual ~TestWorld();
};

} // namespace Test 


#endif // __TestWorld_hxx__

