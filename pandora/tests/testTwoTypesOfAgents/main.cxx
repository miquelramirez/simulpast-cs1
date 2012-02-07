
#include "TestWorld.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <cstdlib>

// this test checks that agents are correctly sent and copied from a node to another one when more than one type exists

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

