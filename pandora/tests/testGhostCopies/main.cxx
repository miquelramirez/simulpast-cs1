
#include "TestWorld.hxx"
#include "Exceptions.hxx"

#include <iostream>
#include <cstdlib>

// this test checks wether all the copy/delete/add operations of agents are reflected on the rest of computer nodes
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

