
#include "TestWorld.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <cstdlib>

// this test checks if agents are correctly updated when we modify them (first from real to ghost, afterwards from ghost to real)
// we create two pairs of agents that move (one couple in vertical, another in horizontal). 
// On even turns, real agents modify a bool value to called "onEvenTurn" to true. 
// On odd turns, agents look for adjacent agents and modify the value to 2

int main(int argc, char *argv[])
{
	try
	{
		Test::TestWorld world;
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

