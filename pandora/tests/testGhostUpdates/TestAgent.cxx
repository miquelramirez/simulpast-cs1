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

#include "TestAgent.hxx"
#include "World.hxx"
#include "Point2D.hxx"

namespace Test
{

TestAgent::TestAgent( const std::string & id, const bool & horizontalMovement ) : Agent(id), _horizontalMovement(horizontalMovement), _evenTurn(true)
{
}

TestAgent::~TestAgent()
{
}

void TestAgent::move()
{
	Engine::Point2D<int> newPosition = _position;
	if(_horizontalMovement)
	{
		newPosition._x++;
	}
	else
	{
		newPosition._y++;
	}
	if(_world->checkPosition(newPosition))
	{
		_position = newPosition;
		return;
	}
}

void TestAgent::updateTurnInformation()
{
	if(_world->getCurrentStep()%2==0)
	{
		//std::cout << "agent: " << this << " on even turn in step: " << _world->getCurrentStep() << std::endl;
		_evenTurn = true;
	}
	else
	{		
		//std::cout << "agent: " << this << " on odd turn in step: " << _world->getCurrentStep() << std::endl;
		Engine::World::AgentsList neighbors = _world->getNeighbours(this, 1);
		for(Engine::World::AgentsList::iterator it=neighbors.begin(); it!=neighbors.end(); it++)
		{
			TestAgent * agent = (TestAgent*)(*it);
			//std::cout << "agent: " << this << " is updating: " << agent << std::endl;
			agent->setEvenTurn(false);
		}
	}
}

void TestAgent::setEvenTurn( const bool & evenTurn )
{
	_evenTurn = evenTurn;
}

bool TestAgent::isEvenTurn()
{
	return _evenTurn;
}

void TestAgent::step()
{
	move();
	updateTurnInformation();
}

void TestAgent::serialize()
{
}

} // namespace Test

