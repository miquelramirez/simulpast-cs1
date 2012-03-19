/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONSN IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "TestWorld.hxx"
#include "Exceptions.hxx"
#include "Simulation.hxx"

#include <iostream>
#include <cstdlib>

// this test checks if initial data is correctly loaded in init time.
// it creates an agent in an overlapping zone in order to check whether its ghosts are created in neighbor worlds
// it checks raster data as well

int main(int argc, char *argv[])
{
	try
	{	
		Engine::Simulation testSim(64, 10);
		Test::TestWorld world(testSim);
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

