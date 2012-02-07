
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
	void registerTypes();
	void * createPackage( const std::string & key );
	void createAgents();
	Engine::Agent * createAgentFromPackage( const std::string & key, void * package );
	void stepAgents();
public:
	TestWorld();
	virtual ~TestWorld();
};

} // namespace Test 


#endif // __TestWorld_hxx__

