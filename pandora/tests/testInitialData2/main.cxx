
#include "TestWorld.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <cstdlib>

// this test checks that all the initial data is being correctly copied to the neighbors in overlap zone

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

