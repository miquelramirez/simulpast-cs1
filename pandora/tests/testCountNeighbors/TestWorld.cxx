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
#include "TestAgent.hxx"
#include <assert.h>
#include <iostream>

namespace Test
{

TestWorld::TestWorld( Engine::Simulation & simulation ) : World( simulation, 9, false, "data/test.h5")
{
}

TestWorld::~TestWorld()
{
}

void TestWorld::stepAgents()
{

	if(_simulation.getNumTasks()==1)
	{
		Engine::Agent * agent = getAgent("test_0");
		Engine::Agent * agent2 = getAgent("test_1");

		// euclidean distance between position (31,31) and (25,25) is sqrt (6*6+6*6) = 8.48528137
		float euclideanDistance = sqrt(6*6+6*6);

		assert(countNeighbours(agent, 7, "test")==0);
		assert(countNeighbours(agent, 8, "test")==0);
		assert(countNeighbours(agent, euclideanDistance, "test")==1);
		assert(countNeighbours(agent, 9, "test")==1);

		Engine::World::AgentsList neighbors = getNeighbours(agent, euclideanDistance , "test");
		Engine::Agent * neighbor = *neighbors.begin();
		assert(neighbors.size()==1);
		assert(neighbor->getId().compare("test_1")==0);

		// with 1 single task you should be able to see the entire set of agents
		assert(countNeighbours(agent,1000, "test")==2);
		assert(countNeighbours(agent,1000)==500);
		return;
	}
	if(_simulation.getId()==0)
	{
		Engine::Agent * agent = getAgent("test_0");
		std::cout << "dist 7: " << countNeighbours(agent,7, "test") << " dist 8: " << countNeighbours(agent,8, "test") << " 9: " << countNeighbours(agent, 9, "test") << " all: " << countNeighbours(agent, 1000, "test") << " --" <<  countNeighbours(agent, 1000) << std::endl;
		assert(countNeighbours(agent,8, "test")==0);
		assert(countNeighbours(agent,9, "test")==3);
	}
	
}

void TestWorld::createRasters()
{
}

void TestWorld::createAgents()
{
	if(_simulation.getId()==0)
	{
		TestAgent * agent = new TestAgent("test_0");
		agent->setPosition(Engine::Point2D<int>(31,31));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_1");
		agent2->setPosition(Engine::Point2D<int>(25,25));
		addAgent(agent2);
		
		TestAgent * agent3 = new TestAgent("test_2");
		agent3->setPosition(Engine::Point2D<int>(0,0));
		addAgent(agent3);

		// we will create several agents in order to test whether selection by type is working
		for(int i=2; i<500; i++)
		{	
			std::ostringstream oss;
			oss << "foo_" << i;
			TestAgent * agentN = new TestAgent(oss.str());
			bool located = false;
			while(!located)
			{
				Engine::Point2D<int> pos(_statistics.getUniformDistValue(0,_boundaries._size._x), _statistics.getUniformDistValue(0,_boundaries._size._y));
				pos += _boundaries._origin;
				if(checkPosition(pos) && _boundaries.isInside(pos))
				{
					agentN->setPosition(pos);
					located = true;
				}
			}
			addAgent(agentN);
		}
		return;
	}
	if(_simulation.getId()==1)
	{
		TestAgent * agent = new TestAgent("test_3");
		agent->setPosition(Engine::Point2D<int>(37,25));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_4");
		agent2->setPosition(Engine::Point2D<int>(63,0));
		addAgent(agent2);

		return;
	}
	if(_simulation.getId()==3)
	{
		TestAgent * agent = new TestAgent("test_5");
		agent->setPosition(Engine::Point2D<int>(37,37));
		addAgent(agent);
		
		TestAgent * agent2 = new TestAgent("test_6");
		agent2->setPosition(Engine::Point2D<int>(63,63));
		addAgent(agent2);

		return;
	}


}

} // namespace Test 

