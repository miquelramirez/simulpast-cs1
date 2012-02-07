
#include "TestWorld.hxx"
#include "Exceptions.hxx"
#include "Simulation.hxx"

#include <iostream>
#include <cstdlib>

// this test checks if count neighbors methods work, using euclidean distances

int main(int argc, char *argv[])
{
	try
	{
		Engine::Simulation sim(64,4);
		Test::TestWorld world(sim);
		world.init(argc, argv);
		world.run();
	}
	catch( std::exception & exceptionThrown )
	{
		std::cout << "exception thrown: " << exceptionThrown.what() << std::endl;
		return -1;
	}
	return 0;
}

