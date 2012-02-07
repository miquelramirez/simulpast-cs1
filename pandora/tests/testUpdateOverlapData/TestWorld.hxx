
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
	void * createPackage( const std::string & type );
	void createAgents();
	Engine::Agent * createAgentFromPackage( const std::string & type, void * package );
public:
	TestWorld();
	virtual ~TestWorld();
	void stepRaster( const std::string & key, Engine::Raster & raster);		
};

} // namespace Test 


#endif // __TestWorld_hxx__

