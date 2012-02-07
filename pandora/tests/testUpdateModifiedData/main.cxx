
#include "TestWorld.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <cstdlib>

// check that data is correctly updated to overlapping nodes when it is modified during running time

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

