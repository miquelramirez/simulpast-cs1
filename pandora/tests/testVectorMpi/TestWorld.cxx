/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
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

#include <TestWorld.hxx>

#include <Exceptions.hxx>
#include <TestAgent.hxx>
#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld( Engine::Simulation & simulation ) : World( simulation, 1, true, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::stepAgents()
{
	std::cout << "checking step agents" << std::endl;
	TestAgent * agent = (TestAgent*)getAgent("TestAgent_0");
	if(agent)
	{
		std::vector<int> & vectorInt = agent->getVectorInt();
		std::vector<float> & vectorFloat = agent->getVectorFloat();
		for(int i=0; i<200; i++)
		{	
			assert(vectorInt[i] == i);
			assert(vectorFloat[i] == i);
		}
	}

	/*
	agent = (TestAgent*)getAgent("test_1");
	if(agent)
	{
		std::vector<int> & vectorInt = agent->getVectorInt();
		std::vector<float> & vectorFloat = agent->getVectorFloat();
		for(int i=0; i<200; i++)
		{	
			assert(vectorInt[199-i] == i);
			assert(vectorFloat[199-i] == i);
		}
	}
	*/
	std::cout << "checking step agents done" << std::endl;
}

void TestWorld::createRasters()
{
}

void TestWorld::createAgents()
{
	if(_simulation.getId()==0)
	{
		TestAgent * agent = new TestAgent("TestAgent_0", true);
		agent->setPosition(Engine::Point2D<int>(0,0));
		agent->getVectorInt().resize(200);
		agent->getVectorFloat().resize(200);
		for( int i=0; i<200; i++)
		{
			agent->getVectorInt()[i] = i;
			agent->getVectorFloat()[i] = i;
		}
		addAgent(agent);
	}
	/*
	if(_simulation.getId()==_simulation.getNumTasks()-1)
	{
		TestAgent * agent = new TestAgent("test_1", false);
		Engine::Point2D<int> pos(_simulation.getSize()-1, _simulation.getSize()-1);
		agent->setPosition(pos);
		agent->getVectorInt().resize(200);
		agent->getVectorFloat().resize(200);
		for(int i=0; i<200; i++)
		{
			agent->getVectorInt()[199-i] = i;
			agent->getVectorFloat()[199-i] = i;
		}
		addAgent(agent);
		
		return;
	}
	*/
}

} // namespace Test 

