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

#ifndef __TestAgentB_hxx__
#define __TestAgentB_hxx__

#include "Agent.hxx"
#include <string>

namespace Test
{

class TestAgentB: public Engine::Agent
{
	int _testValueB; // MpiAttribute

	void move();
	void serialize();

public:
	TestAgentB( const std::string & id );
	virtual ~TestAgentB();
	
	void step();
	const int & getTestValueB();
	
	// Mpi related
	TestAgentB( void * package );
	void * fillPackage();
};

} // namespace Test

#endif // __TestAgentB_hxx__

