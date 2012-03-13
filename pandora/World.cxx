
#include "World.hxx"
#include "Agent.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include "MpiFactory.hxx"

#include <cstdlib>
#include <iostream>
#include <algorithm>

namespace Engine
{

World::World( Simulation & simulation, const int & overlap, const bool & allowMultipleAgentsPerCell, const std::string & fileName ) 
    : _simulation(simulation)
    , _worldPos(-1,-1)
    , _globalBoundaries(Point2D<int>(0,0)
    , Point2D<int>(simulation.getSize(), simulation.getSize()))
    , _allowMultipleAgentsPerCell(allowMultipleAgentsPerCell)
    , _step(0)
    , _overlap(overlap)
    , _initialTime(0.0f)
    , _serializer(fileName)
    , _searchAgents(true)
{

}

World::~World()
{
	AgentsList::iterator it=_agents.begin();
	while(it!=_agents.end())
	{
		Agent * agent = *it;
		it = _agents.erase(it);
		delete agent;
	}
}


void World::init( int argc, char *argv[] )
{
	MPI_Init(&argc, &argv);

	_simulation.init();
	checkOverlapSize();

	_initialTime = MPI_Wtime();
	
	//srand(time(0)*(_simulation.getId()+1));	

	stablishPosition();
	createRasters();
	
	_serializer.init(_simulation, _staticRasters, _dynamicRasters, *this);
	serializeStaticRasters();
	createAgents();
	MpiFactory::instance()->registerTypes();
	std::cout << _simulation.getId() << " finished init at: " << MPI_Wtime() - _initialTime << std::endl;
}

void World::checkOverlapSize()
{
	int subfieldSize = _simulation.getLocalRasterSize()/2;
	if(_overlap*2>subfieldSize)
	{
		std::stringstream oss;
		oss << "World::checkOverlapSize- subfieldize: " << subfieldSize << " must be at lest twice the value of overlap: " << _overlap << " to avoid conflicts between non adjacent subfields";
		throw Exception(oss.str());
	}
}

void World::stablishPosition()
{
	int worldsPerRow = sqrt(_simulation.getNumTasks());	
	if(_simulation.getSize()%worldsPerRow!=0)
	{
		std::stringstream oss;
		oss << "World::stablishPosition - globalRasterSize: " << _simulation.getSize() << " is not divisible by sqrt(worlds): " << worldsPerRow;
		throw Engine::Exception(oss.str());
	}
	_worldPos = getPositionFromId(_simulation.getId());

	for(int x=_worldPos._x-1; x<=_worldPos._x+1; x++)
	{
		for(int y=_worldPos._y-1; y<=_worldPos._y+1; y++)
		{
			if(x>-1 && x<worldsPerRow && y>-1 && y<worldsPerRow)
			{
				if(x!=_worldPos._x || y!=_worldPos._y)
				{
					_neighbors.push_back(y*worldsPerRow+x);
				}
			}
		}
	}

	// stablishing boundaries
	_boundaries._origin = _worldPos*_simulation.getLocalRasterSize();
	_boundaries._size._x = _simulation.getLocalRasterSize();
	_boundaries._size._y = _simulation.getLocalRasterSize();

	// defining overlap boundaries
	_overlapBoundaries = _boundaries;
	// west boundary
	if(_boundaries._origin._x!=_globalBoundaries._origin._x)
	{
		_overlapBoundaries._origin._x -= _overlap;
		_overlapBoundaries._size._x += _overlap;
	}
	// east boundary
	if(_boundaries._origin._x!=_globalBoundaries._size._x-_boundaries._size._x)
	{
		_overlapBoundaries._size._x += _overlap;
	}
	// north boundary
	if(_boundaries._origin._y!=_globalBoundaries._origin._y)
	{
		_overlapBoundaries._origin._y -= _overlap;
		_overlapBoundaries._size._y += _overlap;
	}
	// south boundary
	if(_boundaries._origin._y!=_globalBoundaries._size._y-_boundaries._size._y)
	{
		_overlapBoundaries._size._y += _overlap;
	}

	// creating sections
	_sections.resize(4);
	_sections[0] = Rectangle<int>(_boundaries._origin, _boundaries._size/2);
	_sections[1] = Rectangle<int>(Point2D<int>(_boundaries._origin._x+_boundaries._size._x/2, _boundaries._origin._y), _boundaries._size/2);
	_sections[2] = Rectangle<int>(Point2D<int>(_boundaries._origin._x, _boundaries._origin._y+_boundaries._size._y/2), _boundaries._size/2);
	_sections[3] = Rectangle<int>(Point2D<int>(_boundaries._origin._x+_boundaries._size._x/2, _boundaries._origin._y+_boundaries._size._y/2), _boundaries._size/2);

	std::cout << _simulation.getId() << " has pos: " << _worldPos << ", global boundaries: " << _globalBoundaries << ", boundaries: " << _boundaries << " and overlapped area: " << _overlapBoundaries << std::endl;
	std::cout << _simulation.getId() << " has sections 0:" << _sections[0] << " 1: " << _sections[1] << " 2:" << _sections[2] << " 3: " << _sections[3] << std::endl;
}

void World::updateRasterToMaxValues( const std::string & key )
{
	getDynamicRaster(key).updateRasterToMaxValues();
}

bool World::hasBeenExecuted( Agent * agent )
{
	for(AgentsList::iterator it=_executedAgents.begin(); it!=_executedAgents.end(); it++)
	{
		if((*it)->getId().compare(agent->getId())==0)
		{
			return true;
		}
	}
	return false;
}

bool World::willBeRemoved( Agent * agent )
{
	for(AgentsList::iterator it=_removedAgents.begin(); it!=_removedAgents.end(); it++)
	{	
		if((*it)->getId().compare(agent->getId())==0)
		{
			return true;
		}
	}
	return false;
}

void World::sendGhostAgents( const int & sectionIndex )
{
	std::vector<int> neighborsToUpdate;
	for(int i=0; i<_neighbors.size(); i++)
	{
		if(needsToBeUpdated(_neighbors[i], sectionIndex))
		{
			neighborsToUpdate.push_back(_neighbors[i]);
			//std::cout << _simulation.getId() << " - step: " << _step << "/" << sectionIndex << " will send overlap to: " << _neighbors[i] << std::endl;
		}
	}

	// for each type of agent we will send the collection of agents of the particular type to neighbors
	for(MpiFactory::TypesMap::iterator itType=MpiFactory::instance()->beginTypes(); itType!=MpiFactory::instance()->endTypes(); itType++)
	{
		MPI_Datatype * agentType = itType->second;

		std::vector< AgentsList > agentsToNeighbors;
		agentsToNeighbors.resize(neighborsToUpdate.size());

		for(int i=0; i<neighborsToUpdate.size(); i++)
		{
			Rectangle<int> overlapZone = getOverlap(neighborsToUpdate[i], sectionIndex);
			for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
			{
				Agent * agent = *it;
				// we check the type. TODO register the type in another string
				// TODO refactor!!!
				if(agent->isType(itType->first))
				{
					if((!willBeRemoved(agent)) && (overlapZone.isInside(agent->getPosition()-_overlapBoundaries._origin)))
					{
						agentsToNeighbors[i].push_back(*it);
						//std::cout << _simulation.getId() << " will send ghost agent: " << agent << " to: " << neighborsToUpdate[i] << " in section index: " << sectionIndex << std::endl;
					}
				}
			}
			for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)
			{
				Agent * agent = *it;	
				if(agent->isType(itType->first))
				{
					if((!willBeRemoved(agent)) && (overlapZone.isInside(agent->getPosition()-_overlapBoundaries._origin)))
					{
						agentsToNeighbors[i].push_back(*it);
						//std::cout << _simulation.getId() << " will send modified ghost agent: " << agent << " to: " << neighborsToUpdate[i] << " in section index: " << sectionIndex << " and step: " << _step << std::endl;
					}
				}
			}

			int numAgents = agentsToNeighbors[i].size();
			//std::cout << _simulation.getId() << " sending num ghost agents: " << numAgents << " to : " << neighborsToUpdate[i] << " in step: " << _step << " and section index: " << sectionIndex <<  std::endl;
			int error = MPI_Send(&numAgents, 1, MPI_INTEGER, neighborsToUpdate[i], 4, MPI_COMM_WORLD);
			if(error != MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::sendGhostAgents - " << _simulation.getId() << " error in MPI_Send : " << error;
				throw Exception(oss.str());
			}
			for(AgentsList::iterator it=agentsToNeighbors[i].begin(); it!=agentsToNeighbors[i].end(); it++)
			{
				Agent * agent = *it;
				void * package = agent->fillPackage();
				//std::cout << _simulation.getId() << " sending ghost agent: " << *it << " from: " << _simulation.getId() << " to: " << neighborsToUpdate[i] << std::endl;
				error = MPI_Send(package, 1, *agentType, neighborsToUpdate[i], 5, MPI_COMM_WORLD);
				delete package;
				if(error != MPI_SUCCESS)
				{
					std::stringstream oss;
					oss << "World::sendGhostAgents - " << _simulation.getId() << " error in MPI_Send : " << error;
					throw Exception(oss.str());
				}
			}
		}
	}
}

void World::receiveGhostAgents( const int & sectionIndex )
{
	// we need to calculate how many neighbors will send data to this id
	std::vector<int> neighborsToUpdate;
	for(int i=0; i<_neighbors.size(); i++)
	{
		if(needsToReceiveData(_neighbors[i], sectionIndex))
		{
			neighborsToUpdate.push_back(_neighbors[i]);
		}
	}

	// for each type of agent we will send the collection of agents of the particular type to neighbors	
	for(MpiFactory::TypesMap::iterator itType=MpiFactory::instance()->beginTypes(); itType!=MpiFactory::instance()->endTypes(); itType++)
	{
		MPI_Datatype * agentType = itType->second;

		for(int i=0; i<neighborsToUpdate.size(); i++)
		{
			AgentsList newGhostAgents;
			int numAgentsToReceive;
			MPI_Status status;
			int error = MPI_Recv(&numAgentsToReceive, 1, MPI_INTEGER, neighborsToUpdate[i], 4, MPI_COMM_WORLD, &status);			
			if(error!=MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::receiveGhostAgents - " << _simulation.getId() << " error in MPI_Recv: " << error;
				throw Exception(oss.str());
			}
			//std::cout << _simulation.getId() << " has received message from " << neighborsToUpdate[i] << ", num ghost agents: " << numAgentsToReceive << std::endl;
			for(int j=0; j<numAgentsToReceive; j++)
			{
				void * package = MpiFactory::instance()->createDefaultPackage(itType->first);
				error = MPI_Recv(package, 1, *agentType, neighborsToUpdate[i], 5, MPI_COMM_WORLD, &status);					
				if(error!=MPI_SUCCESS)
				{
					std::stringstream oss;
					oss << "World::receiveGhostAgents - " << _simulation.getId() << " error in MPI_Recv: " << error;
					throw Exception(oss.str());
				}
				Agent * agent = MpiFactory::instance()->createAndFillAgent(itType->first, package);
				//std::cout << _simulation.getId() << " has received ghost agent: " << agent << " number: " << j << " from: " << neighborsToUpdate[i] << " in section index: " << sectionIndex << " and step: " << _step << std::endl;
				delete package;
				// we must check if it is an update of an agent, or a ghost agent
				bool worldOwnsAgent = false;
				AgentsList::iterator it = getOwnedAgent(agent->getId());
				if(it!=_agents.end())
				{
					//std::cout << _simulation.getId() << " has received update of own agent: " << *it << " in step: " << _step << std::endl;
					_agents.erase(it);
					addAgent(agent);
					worldOwnsAgent = true;
				}
				if(!worldOwnsAgent)
				{
					newGhostAgents.push_back(agent);
				}
			}
			//std::cout << _simulation.getId() << " num ghost agents sent for neighbor: " << neighborsToUpdate[i] << " in section index: " << sectionIndex << ": " << newGhostAgents.size() << std::endl;
			// if the agent is in the zone to be updated, remove it
			Rectangle<int> overlapZone = getOverlap(neighborsToUpdate[i], sectionIndex);
			AgentsList::iterator it=_overlapAgents.begin();
			while(it!=_overlapAgents.end())
			{
				Agent * agent = (*it);
				if(agent->isType(itType->first))
				{
					// si l'agent no està en zona que s'ha d'actualitzar, continuar
					if(overlapZone.isInside((*it)->getPosition()-_overlapBoundaries._origin))
					{
						//std::cout << _simulation.getId() << " in section index: " << sectionIndex << " with overlap zone: " << overlapZone << " erasing agent: " << *it << std::endl;
						it = _overlapAgents.erase(it);
					}
					else
					{
						//std::cout << _simulation.getId() << " in section index: " << sectionIndex <<  " with overlap zone: " << overlapZone << " maintaining agent: " << *it << std::endl;
						it++;
					}
				}
				else
				{
					it++;
				}
			}
			// afterwards we will add the new ghost agents
			for(it=newGhostAgents.begin(); it!=newGhostAgents.end(); it++)
			{
				//std::cout << _simulation.getId() << " in section index: " << sectionIndex << " adding ghost agent: " << *it << std::endl;
				_overlapAgents.push_back(*it);
			}
		}
	}
}

void World::addAgent( Agent * agent )
{
	agent->setWorld(this);
	// we need to remove any previous instance of this agent in overlap

	AgentsList::iterator it = getGhostAgent(agent->getId());
	if(it!=_overlapAgents.end())
	{
		_overlapAgents.erase(it);
	}
	_agents.push_back(agent);
}

void World::stepSection( const int & sectionIndex )
{
	// first of all we will execute raster rules for the zone where stepSection is executed
	stepRastersSection(sectionIndex, _sections[sectionIndex]);

	//std::cout << _simulation.getId() << " executing section: " << sectionIndex << " in step: " << _step << " with zone: " << _sections[sectionIndex] << std::endl;
	AgentsList::iterator it=_agents.begin();
	std::vector<Agent*> agentsToExecute;
	// we have to randomize the execution of agents in a given section index
	while(it!=_agents.end())
	{
		Agent * agent = *it;
		if(_sections[sectionIndex].isInside(agent->getPosition()) && !hasBeenExecuted(agent))
		{
			agentsToExecute.push_back(agent);
		}
		it++;
	}
	std::random_shuffle(agentsToExecute.begin(), agentsToExecute.end());
	int numExecutedAgents = 0;
	AgentsList agentsToSend;

#pragma omp for
	for(int i=0; i<agentsToExecute.size(); i++)
	{
//	while(it!=_agents.end())
//	{
		Agent * agent = agentsToExecute[i];
		//Agent * agent = *it;
		if(_sections[sectionIndex].isInside(agent->getPosition()) && !hasBeenExecuted(agent))
		{
			//std::cout << "agent: " << agent << " being executed at index: " << sectionIndex << " of task: "<< _simulation.getId() << " in step: " << _step << std::endl;
			agent->step();
			//std::cout << "agent: " << agent << " has been executed at index: " << sectionIndex << " of task: "<< _simulation.getId() << " in step: " << _step << std::endl;
			if(!_boundaries.isInside(agent->getPosition()) && !willBeRemoved(agent))
			{
				//std::cout << "migrating agent: " << agent << " being executed at index: " << sectionIndex << " of task: "<< _simulation.getId() << std::endl;
				agentsToSend.push_back(agent);

				// the agent is no longer property of this world
				it = getOwnedAgent(agent->getId());
				//it = _agents.erase(it);
				_agents.erase(it);
				// it will be deleted				
				_overlapAgents.push_back(agent);
				//std::cout << _simulation.getId() << " putting agent: " << agent << " to overlap" << std::endl;
			}
			else
			{
				//std::cout << _simulation.getId() << " finished agent: " << agent << std::endl;
				it++;
			}
			_executedAgents.push_back(agent);
			numExecutedAgents++;
		}
		/*
		else
		{
			//std::cout << _simulation.getId() << " agent: " << agent << " not executed" << std::endl;
			it++;
		}
		*/
	}

	//std::cout << _simulation.getId() << " will send agents in section: " << sectionIndex << " and step: " << _step << std::endl;
	sendAgents(agentsToSend);
	//std::cout << _simulation.getId() << " has finished section: " << sectionIndex << " and step: " << _step << std::endl;
	
	//std::cout << MPI_Wtime() - _initialTime<< " - world: " << _simulation.getId() << " at pos: " << _worldPos << " has executed step: " << _step << " section: " << sectionIndex << " in zone: " << _sections[sectionIndex] << " with num executed agents: " << numExecutedAgents << " total agents: " << _agents.size() << " and overlap agents: " << _overlapAgents.size() << std::endl;
}

void World::serializeAgents()
{
	AgentsList::iterator it=_agents.begin();
	while(it!=_agents.end())
	{
		if((*it)->exists())
		{
			_serializer.serializeAgent((*it), _step);
		}
		it++;
	}
}

void World::sendAgents( AgentsList & agentsToSend )
{
	if(_neighbors.size()==0)
	{
		return;
	}
	//std::cout << _simulation.getId() << " BEGIN checking agents to send: " << agentsToSend.size() << " with: " << _neighbors.size() << " neighbors" << std::endl;

	//TODO: es poden enviar missatges de forma no sincronitzada ni predestinada?



//	MPI_Datatype * agentType = createType();
	// for each neighbor, we send the number of agents to send
	for(MpiFactory::TypesMap::iterator itType=MpiFactory::instance()->beginTypes(); itType!=MpiFactory::instance()->endTypes(); itType++)
	{
		// add each agent to the list of the neighbour where it will be sent
		std::vector< AgentsList > agentsToNeighbors;
		agentsToNeighbors.resize(_neighbors.size());


		for(AgentsList::iterator it=agentsToSend.begin(); it!=agentsToSend.end(); it++)
		{
			Agent * agent = (*it);
			if(agent->isType(itType->first))
			{
				int newID = getIdFromPosition(agent->getPosition());
				agentsToNeighbors[getNeighborIndex(newID)].push_back(agent);
			}
		}

		MPI_Datatype * agentType = itType->second;
		for(int i=0; i<_neighbors.size(); i++)
		{	
			int numAgents = agentsToNeighbors[i].size();
			//std::cout << _simulation.getId() << " sending num agents: " << numAgents << " to : " << _neighbors[i] << std::endl;
			int error = MPI_Send(&numAgents, 1, MPI_INTEGER, _neighbors[i], 1, MPI_COMM_WORLD);
			if(error != MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::sendAgents - " << _simulation.getId() << " error in MPI_Send : " << error;
				throw Exception(oss.str());
			}
			AgentsList::iterator it= agentsToNeighbors[i].begin();
			while(it!=agentsToNeighbors[i].end())
			{
				Agent * agent = *it;
				void * package = agent->fillPackage();
				//std::cout << _simulation.getId() << " sending agent: " << *it << " from: " << _simulation.getId() << " to: " << _neighbors[i] << std::endl;
				error = MPI_Send(package, 1, *agentType, _neighbors[i], 2, MPI_COMM_WORLD);
				delete package;
				if(error != MPI_SUCCESS)
				{
					std::stringstream oss;
					oss << "World::sendAgents - " << _simulation.getId() << " error in MPI_Send : " << error;
					throw Exception(oss.str());
				}
				it = agentsToNeighbors[i].erase(it);
				// it is not deleted, as it is sent to overlap list
			}
		}
	}
	//std::cout << _simulation.getId() << " END checking agents to send: " << agentsToSend.size() << std::endl;
}

void World::sendOverlapZones( const int & sectionIndex, const bool & entireOverlap )
{
	std::vector<int> neighborsToUpdate;
	for(int i=0; i<_neighbors.size(); i++)
	{
		if(needsToBeUpdated(_neighbors[i], sectionIndex))
		{
			neighborsToUpdate.push_back(_neighbors[i]);
			//std::cout << _simulation.getId() << " - step: " << _step << "/" << sectionIndex << " will send overlap to: " << _neighbors[i] << std::endl;
		}
	}

	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		for(int i=0; i<neighborsToUpdate.size(); i++)
		{
			Rectangle<int> overlapZone;
			if(entireOverlap)
			{
				overlapZone = getOverlap(neighborsToUpdate[i], sectionIndex);
			}
			else
			{
				overlapZone = getInternalOverlap(neighborsToUpdate[i]);
			}
			int dataSize = overlapZone._size._x * overlapZone._size._y;
			int data[dataSize];
			//std::cout << _simulation.getId() << " - step: " << _step << "/" << sectionIndex << " will send overlap to: " << neighborsToUpdate[i] << " with zone: " << overlapZone << std::endl;
			for(int n=0; n<dataSize; n++)
			{
				Point2D<int> index(overlapZone._origin._x+n%overlapZone._size._x, overlapZone._origin._y+n/overlapZone._size._x);
				data[n] = getDynamicRaster(it->first).getValue(index);
				//std::cout << "\t" << _simulation.getId() << " sending data : " << dataSize << " to: " << neighborsToUpdate[i] << " - send index: " << index << " in global pos: " << index+_overlapBoundaries._origin << " value: " << data[n] << " section index: " << sectionIndex << " and step: " << _step << std::endl;
			}
			if(MPI_Send(&data, dataSize, MPI_INTEGER, neighborsToUpdate[i], 3, MPI_COMM_WORLD)!= MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::sendOverlapZones- " << _simulation.getId() << " error in MPI_Send";
				throw Exception(oss.str());
			}
		}
	}
}

void World::sendMaxOverlapZones()
{
	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		for(int i=0; i<_neighbors.size(); i++)
		{
			Rectangle<int> overlapZone = getInternalOverlap(_neighbors[i]);
			int dataSize = overlapZone._size._x * overlapZone._size._y;
			int data[dataSize];
			//std::cout << _simulation.getId() << " - step: " << _step << " will send overlap to: " << _neighbors[i] << " with zone: " << overlapZone << std::endl;
			for(int n=0; n<dataSize; n++)
			{
				Point2D<int> index(overlapZone._origin._x+n%overlapZone._size._x, overlapZone._origin._y+n/overlapZone._size._x);
				data[n] = getDynamicRaster(it->first).getMaxValueAt(index);
				//std::cout << "\t" << _simulation.getId() << " sending max data : " << dataSize << " to: " << _neighbors[i] << " - send index: " << index << " in global pos: " << index+_overlapBoundaries._origin << " value: " << data[n] << " and step: " << _step << std::endl;
			}
			if(MPI_Send(&data, dataSize, MPI_INTEGER, _neighbors[i], 6, MPI_COMM_WORLD)!= MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::sendMaxOverlapZones- " << _simulation.getId() << " error in MPI_Send";
				throw Exception(oss.str());
			}
		}
	}
}


int World::getIdFromPosition( const Point2D<int> & position )
{
	Point2D<int> nodePosition = position/_boundaries._size;
	return nodePosition._y*sqrt(_simulation.getNumTasks())+nodePosition._x;
}

Point2D<int> World::getPositionFromId( const int & id )
{
	int worldsPerRow = sqrt(_simulation.getNumTasks());	
	Point2D<int> worldPos(id%worldsPerRow, id/worldsPerRow);
	return worldPos;
}

int World::getNeighborIndex( const int & id )
{
	for(int i=0; i<_neighbors.size(); i++)
	{
		if(_neighbors[i] == id)
		{
			return i;
		}
	}
	std::stringstream oss;
	oss << "World::getNeighborIndex- " << _simulation.getId() << " looking for neighbor with id: " << id << " not found";
	throw Exception(oss.str());
}

void World::receiveAgents( const int & sectionIndex )
{
	//MPI_Datatype * agentType = createType();
	for(MpiFactory::TypesMap::iterator itType=MpiFactory::instance()->beginTypes(); itType!=MpiFactory::instance()->endTypes(); itType++)
	{
		MPI_Datatype * agentType = itType->second;

		for(int i=0; i<_neighbors.size(); i++)
		{
			int numAgentsToReceive;
			MPI_Status status;
			int error = MPI_Recv(&numAgentsToReceive, 1, MPI_INTEGER, _neighbors[i], 1, MPI_COMM_WORLD, &status);			
			if(error!=MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::receiveAgents - " << _simulation.getId() << " error in MPI_Recv: " << error;
				throw Exception(oss.str());
			}
			//std::cout << _simulation.getId() << " has received message from " << _neighbors[i] << ", num agents: " << numAgentsToReceive << std::endl;
			for(int j=0; j<numAgentsToReceive; j++)
			{
				void * package = MpiFactory::instance()->createDefaultPackage(itType->first);
				error = MPI_Recv(package, 1, *agentType, _neighbors[i], 2, MPI_COMM_WORLD, &status);					
				if(error!=MPI_SUCCESS)
				{
					std::stringstream oss;
					oss << "World::receiveAgents - " << _simulation.getId() << " error in MPI_Recv: " << error;
					throw Exception(oss.str());
				}
				Agent * agent = MpiFactory::instance()->createAndFillAgent(itType->first, package);
				//std::cout << _simulation.getId() << " has received agent: " << agent << " number: " << j << " from: " << _neighbors[i] << std::endl;
				delete package;
				addAgent(agent);
			}
		}
	}
}

void World::receiveOverlapData( const int & sectionIndex, const bool & entireOverlap )
{
	// we need to calculate how many neighbors will send data to this id
	std::vector<int> neighborsToUpdate;
	for(int i=0; i<_neighbors.size(); i++)
	{
		if(needsToReceiveData(_neighbors[i], sectionIndex))
		{
			neighborsToUpdate.push_back(_neighbors[i]);
			//std::cout << _simulation.getId() << " - step: " << _step << "/" << sectionIndex << " will receive overlap from: " << _neighbors[i] << std::endl;
		}
	}

	// for each raster, we receive data from all the active neighbors
	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		for(int i=0; i<neighborsToUpdate.size(); i++)
		{
			Rectangle<int> overlapZone;
			if(entireOverlap)
			{
				overlapZone = getOverlap(neighborsToUpdate[i], sectionIndex);
			}
			else
			{
				overlapZone = getExternalOverlap(neighborsToUpdate[i]);
			}

			int dataSize = overlapZone._size._x*overlapZone._size._y;	
			int data[dataSize];
			//std::cout << _simulation.getId() << " will receive from: " << neighborsToUpdate[i] << " dataSize: " << dataSize << " with overlap: " << overlapZone << " in section index: " << sectionIndex << std::endl;
			MPI_Status status;
			int error = MPI_Recv(&data, dataSize, MPI_INTEGER, neighborsToUpdate[i], 3, MPI_COMM_WORLD, &status);					
			if(error!=MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::step- " << _simulation.getId() << " error in MPI_Recv: " << error;
				throw Exception(oss.str());
			}
			for(int n=0; n<dataSize; n++)
			{
				Point2D<int> index(overlapZone._origin._x+n%overlapZone._size._x, overlapZone._origin._y+n/overlapZone._size._x);
				/*
				if(getDynamicRaster(it->first).getMaxValue(index)<data[n])
				{
					getDynamicRaster(it->first).setMaxValue(index, data[n]);
				}
				*/
				//std::cout << "\t" << _simulation.getId() << " receiving data : " << dataSize << " from: " << neighborsToUpdate[i] << " - index: " << index << " in global pos: " << index+_overlapBoundaries._origin << " value: " << data[n] << " section index: " << sectionIndex << " and step: " << _step << std::endl;
				getDynamicRaster(it->first).setValue(index, data[n]);
			}
		}
	}
}

void World::receiveMaxOverlapData()
{
	// for each raster, we receive data from all the active neighbors
	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		for(int i=0; i<_neighbors.size(); i++)			
		{
			Rectangle<int> overlapZone = getExternalOverlap(_neighbors[i]);
			int dataSize = overlapZone._size._x*overlapZone._size._y;	
			int data[dataSize];
			//std::cout << _simulation.getId() << " will receive max values from: " << _neighbors[i] << " dataSize: " << dataSize << " with overlap: " << overlapZone <<  std::endl;
			MPI_Status status;
			int error = MPI_Recv(&data, dataSize, MPI_INTEGER, _neighbors[i], 6, MPI_COMM_WORLD, &status);					
			if(error!=MPI_SUCCESS)
			{
				std::stringstream oss;
				oss << "World::step- " << _simulation.getId() << " error in MPI_Recv: " << error;
				throw Exception(oss.str());
			}
			for(int n=0; n<dataSize; n++)
			{
				Point2D<int> index(overlapZone._origin._x+n%overlapZone._size._x, overlapZone._origin._y+n/overlapZone._size._x);
				//std::cout << "\t" << _simulation.getId() << " receiving max data : " << dataSize << " from: " << _neighbors[i] << " - index: " << index <<  " in global pos: " << index+_overlapBoundaries._origin << " value: " << data[n] << in step: " << _step << std::endl;
				getDynamicRaster(it->first).setMaxValue(index, data[n]);
			}
		}
	}
}


void World::step()
{
	std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << std::endl;
	// actualitzar el món (raster i agents), si s'escau
	serializeRasters();
	//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " has serialized rasters" << std::endl;	
	serializeAgents();
	//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " has serialized agents" << std::endl;	
	// TODO if we modify agents in this step, they won't be updated in other nodes!
	stepAgents();
	//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " has executed stepAgents" << std::endl;	
	// TODO the same with raster modification?
	stepEnvironment();
	for(int sectionIndex=0; sectionIndex<4; sectionIndex++)
	{
		// section index doesn't matter if is the entire overlap
		// TODO refactor? we are sending 4 times all the info
		sendOverlapZones(sectionIndex, false);
		receiveOverlapData(sectionIndex, false);
	}
	//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " has executed stepAgents" << std::endl;	
	// TODO shuffle
	//random_shuffle(_agents.begin(), _agents.end());
	_executedAgents.clear();

	for(int sectionIndex=0; sectionIndex<4; sectionIndex++)
	{
		stepSection(sectionIndex);

		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " has been executed" << std::endl;	
		receiveAgents(sectionIndex);
		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " has received agents" << std::endl;	

		sendGhostAgents(sectionIndex);
		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " sent ghosts" << std::endl;	
		receiveGhostAgents(sectionIndex);
		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " received ghosts" << std::endl;

		sendOverlapZones(sectionIndex);
		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " sent overlap" << std::endl;
		receiveOverlapData(sectionIndex);
		//std::cout << MPI_Wtime() - _initialTime << " - world: " << _simulation.getId() << " at pos: " << _worldPos << " executing step: " << _step << " and section: " << sectionIndex << " received overlap" << std::endl;

		//MPI_Barrier(MPI_COMM_WORLD);
	}
	removeAgents();
	std::cout << MPI_Wtime() - _initialTime<< " - world: " << _simulation.getId() << " at pos: " << _worldPos << " has executed step: " << _step << std::endl;
}

void World::run()
{
	std::cout << MPI_Wtime() - _initialTime << " - " << _simulation.getId() << " executing " << _simulation.getNumSteps() << " steps..." << std::endl;

	// we need to send the agents and data in overlap zone to adjacent computer nodes	
	sendMaxOverlapZones();
	receiveMaxOverlapData();
	

	for(int sectionIndex=0; sectionIndex<4; sectionIndex++)
	{
		// section index doesn't matter if is the entire overlap
		// TODO refactor? we are sending 4 times all the info
		sendOverlapZones(sectionIndex, false);
		receiveOverlapData(sectionIndex, false);

		sendGhostAgents(sectionIndex);
		receiveGhostAgents(sectionIndex);
	}

	for(_step=0; _step<_simulation.getNumSteps(); _step++)
	{
		step();
	}
	// storing last step data
	serializeRasters();
	serializeAgents();
	
	std::cout << _simulation.getId() << " closing files at: " << MPI_Wtime() - _initialTime << std::endl;
	_serializer.finish();

	MpiFactory::instance()->cleanTypes();
	std::cout << _simulation.getId() << " finished simulation at: " << MPI_Wtime() - _initialTime << std::endl; 
	MPI_Finalize();
}

World::AgentsList::iterator World::getOwnedAgent( const std::string & id )
{
	for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
	{
		if((*it)->getId().compare(id)==0)
		{
			return it;
		}
	}
	return _agents.end();
}

World::AgentsList::iterator World::getGhostAgent( const std::string & id )
{
	for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)		
	{
		if((*it)->getId().compare(id)==0)
		{
			return it;
		}
	}	
	return _overlapAgents.end();
}

Agent * World::getAgent( const std::string & id )
{
	AgentsList::iterator it = getOwnedAgent(id);
	if(it!=_agents.end())
	{
		Agent * agent = (*it);
		if(agent->exists())
		{
			return agent;
		}
	}
	it = getGhostAgent(id);
	if(it!=_agents.end())
	{
		Agent * agent = (*it);
		if(agent->exists())
		{
			return agent;
		}
	}
	return 0;
}

Agent * World::getAgent( const Point2D<int> & position )
{
	for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
	{
		if((*it)->getPosition().isEqual(position))
		{
			return *it;
		}
	}
	for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)		
	{
		if((*it)->getPosition().isEqual(position))
		{
			return *it;
		}
	}
	return 0;
}

/*
void World::agentModified( Agent * agent )
{
	_modifiedGhostAgents.push_back(agent);
}
*/
/*
World::AgentsList World::getAgentsNear( const Position<int> & position, const int & radius, const bool & includeCenter )
{	
	AgentsList agents;
	if(_searchAgents)
	{
		for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
		{
			Agent * agent = (*it);
			// we are not using distance method of Position, as this is not a Manhattan distance
			int distanceX = std::abs(agent->getPosition()._x - position._x);
			int distanceY = std::abs(agent->getPosition()._y - position._y);
			if(distanceX <= radius && distanceY <= radius)
			{
				// if include center and is the same point
				if(!includeCenter)
				{
					if(distanceX!=0 || distanceY!=0)
					{
						agents.push_back(agent);
					}
				}
				else
				{
					agents.push_back(agent);
				}
			}
		}
		for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)
		{
			Agent * agent = (*it);
			// we are not using distance method of Position, as this is not a Manhattan distance
			int distanceX = std::abs(agent->getPosition()._x - position._x);
			int distanceY = std::abs(agent->getPosition()._y - position._y);
			if(distanceX <= radius && distanceY <= radius)
			{
				// if include center and is the same point
				if(!includeCenter)
				{
					if(distanceX!=0 || distanceY!=0)
					{
						agents.push_back(agent);
					}
				}
				else
				{
					agents.push_back(agent);
				}
			}
		}

	}
	else
	{
		Position<int> index;
		for(index._x=position._x-radius; index._x!=position._x+radius+1; index._x++)
		{
			for(index._y=position._y-radius; index._y!=position._y+radius+1; index._y++)
			{
				//std::cout << _simulation.getId() << " checking near agents: " << index << " from pos: " << position << " with radius: " << radius << std::endl;
				if(includeCenter || index!=position)
				{
					Agent * agent = getAgent(index);
					if(agent)
					{
						//std::cout << _simulation.getId() << " agent: " << agent << " is neighbor of position: " << position << std::endl;
						agents.push_back(agent);
					}
				}
			}
		}
	}
	return agents;
}
*/
const int & World::getCurrentStep() const
{
	return _step;
}

void World::serializeRasters()
{
	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		if(rasterToSerialize(it->first))
		{
			_serializer.serializeRaster(it->first, it->second, *this, _step);
		}
	}
}

void World::serializeStaticRasters()
{
	for(StaticRastersMap::iterator it=_staticRasters.begin(); it!=_staticRasters.end(); it++)
	{
		if(rasterToSerialize(it->first))
		{
			_serializer.serializeStaticRaster(it->first, it->second, *this);
		}
	}
}

void World::stepEnvironment()
{
	for(RastersMap::iterator it=_dynamicRasters.begin(); it!=_dynamicRasters.end(); it++)
	{
		// TODO initial pos and matrix size are needed?
		stepRaster(it->first);
	}
}

void World::stepRastersSection(  const int & indexSection, const Rectangle<int> & section )
{
}

void World::stepRaster( const std::string & key)
{
	getDynamicRaster(key).updateRasterIncrement();
}

void World::stepAgents()
{
}

void World::registerDynamicRaster( const std::string & key, const bool & serialize )
{
	_dynamicRasters.insert( make_pair( key, Raster()));
	getDynamicRaster(key).resize(_overlapBoundaries._size);
	if(serialize)
	{
		_rastersToSerialize.push_back(key);
	}
}

void World::registerStaticRaster( const std::string & key, const bool & serialize )
{
	_staticRasters.insert( make_pair( key, StaticRaster()));
	getStaticRaster(key).resize(_overlapBoundaries._size);
	if(serialize)
	{
		_rastersToSerialize.push_back(key);
	}
}
bool World::checkPosition( const Point2D<int> & newPosition )
{
	// checking global boundaries: if environment is a border of the real world
	if(!_globalBoundaries.isInside(newPosition))
	{
		//std::cout << " not valid position: " << newPosition << std::endl;
		return false;
	}

	if(_allowMultipleAgentsPerCell)
	{
		return true;
	}
	
	// checking if it is already occupied
	Agent * host = getAgent(newPosition);
	if(host && host->exists())
	{
		return false;
	}
	return true;
}

Statistics & World::getStatistics()
{
	return _statistics;
}

Simulation & World::getSimulation()
{
	return _simulation;
}

StaticRaster & World::getStaticRaster( const std::string & key )
{
	StaticRastersMap::iterator it = _staticRasters.find(key);
	if(it==_staticRasters.end())
	{
		// the key does not exists	
		std::stringstream oss;
		oss << "World::getStaticRaster - searching for unregistered raster: " << key;
		throw Engine::Exception(oss.str());
	}
	return it->second;
}

Raster & World::getDynamicRaster( const std::string & key )
{
	RastersMap::iterator it = _dynamicRasters.find(key);
	if(it==_dynamicRasters.end())		
	{
		// the key does not exists	
		std::stringstream oss;
		oss << "World::getDynamicRaster - searching for unregistered raster: " << key;
		throw Exception(oss.str());
	}
	return it->second;
}

StaticRaster & World::getRasterTmp( const std::string & key )
{
	RastersMap::iterator it = _dynamicRasters.find(key);
	if(it!=_dynamicRasters.end())		
	{
		return it->second;
	}
	StaticRastersMap::iterator itS = _staticRasters.find(key);
	if(itS!=_staticRasters.end())
	{
		return itS->second;
	}
	// the key does not exists	
	std::stringstream oss;
	oss << "World::getDynamicRasterTmp - searching for unregistered raster: " << key;
	throw Exception(oss.str());
}
void World::setValue( const std::string & key, const Point2D<int> & position, int value )
{
	Point2D<int> localPosition(position - _overlapBoundaries._origin);
	//std::cout << _simulation.getId() << " with boundaries: " << _boundaries << " is setting global pos: " << position << " and value: " << value << " to local: " << localPosition << std::endl;
	getDynamicRaster(key).setValue(localPosition, value);
}

int World::getValue( const std::string & key, const Point2D<int> & position )
{
	Point2D<int> localPosition(position - _overlapBoundaries._origin);
	return getRasterTmp(key).getValue(localPosition);
}

void World::setMaxValue( const std::string & key, const Point2D<int> & position, int value )
{
	Point2D<int> localPosition(position - _overlapBoundaries._origin);
	//std::cout << _simulation.getId() << " with boundaries: " << _boundaries << " is setting global pos: " << position << " and value: " << value << " to local: " << localPosition << std::endl;
	getDynamicRaster(key).setMaxValue(localPosition, value);
}

int World::getMaxValueAt( const std::string & key, const Point2D<int> & position )
{
	Point2D<int> localPosition(position - _overlapBoundaries._origin);	
	return getDynamicRaster(key).getMaxValueAt(localPosition);
}

const Rectangle<int> & World::getBoundaries()
{
	return _boundaries;
}

const Rectangle<int> & World::getOverlapBoundaries()
{
	return _overlapBoundaries;
}


bool World::isCorner( const int & neighbor )
{
	Point2D<int> worldPos = getPositionFromId(neighbor);
	Point2D<int> diff = worldPos - _worldPos;
	if(std::abs(diff._x)==1 && std::abs(diff._y)==1)
	{
		return true;
	}
	return false;
}

Rectangle<int> World::getInternalOverlap( const int & id )
{
	Point2D<int> diff = getPositionFromId(id)-_worldPos;
	// left
	
	Rectangle<int> result;

	// origin
	if(diff._x==-1)
	{
		if(_boundaries._origin._x==0)
		{
			result._origin._x = 0;
		}
		else
		{
			result._origin._x = _overlap;
		}
	}
	else if(diff._x==0)
	{
		if(_boundaries._origin._x==0)
		{
			result._origin._x = 0;
		}
		else
		{
			result._origin._x = _overlap;
		}
	}
	else
	{
		result._origin._x = _boundaries._size._x - _overlap;
	}
	
	if(diff._y==-1)
	{
		if(_boundaries._origin._y==0)
		{
			result._origin._y = 0;
		}
		else
		{
			result._origin._y = _overlap;
		}
	}
	else if(diff._y==0)
	{
		if( _boundaries._origin._y==0)
		{
			result._origin._y = 0;
		}
		else
		{
			result._origin._y = _overlap;
		}
	}
	else
	{
		result._origin._y = _boundaries._size._y - _overlap;
	}

	// size
	if(diff._x!=0)
	{
		result._size._x = _overlap;
	}
	else
	{
		result._size._x = _boundaries._size._x;
	}
	
	if(diff._y!=0)
	{
		result._size._y = _overlap;
	}
	else
	{
		result._size._y = _boundaries._size._y;
	}
	//std::cout << "internal overlap between: " << _simulation.getId()  << " with world pos: " << _worldPos << " and neighbor: " << id << " and diff: " << diff << " in section index: " << sectionIndex << " is: " << result << std::endl;
	return result;
}

Rectangle<int> World::getExternalOverlap( const int & id)
{
	Point2D<int> diff = getPositionFromId(id)-_worldPos;
	// left
	
	Rectangle<int> result;

	// origin
	if(diff._x==-1)
	{
		result._origin._x = 0;
	}
	else if(diff._x==0)
	{
		if(_boundaries._origin._x == 0)
		{
			result._origin._x = 0;
		}
		else
		{
			result._origin._x = _overlap;
		}
	}
	else
	{
		result._origin._x = _boundaries._size._x;
	}
	
	if(diff._y==-1)
	{	
		result._origin._y = 0;
	}
	else if(diff._y==0)
	{
		if(_boundaries._origin._y == 0)
		{
			result._origin._y = 0;
		}
		else
		{
			result._origin._y = _overlap;
		}
	}
	else
	{
		result._origin._y = _boundaries._size._y;
	}

	// size
	if(diff._x!=0)
	{
		result._size._x = _overlap;
	}
	else
	{
		result._size._x = _boundaries._size._x;
	}
	
	if(diff._y!=0)
	{
		result._size._y = _overlap;
	}
	else
	{	
		result._size._y = _boundaries._size._y;
	}
	//std::cout << "external overlap between: " << _simulation.getId()  << " with world pos: " << _worldPos << " and neighbor: " << id << " and diff: " << diff << " in section index: " << sectionIndex << " is: " << result << std::endl;
	return result;
}

Rectangle<int> World::getOverlap( const int & id, const int & sectionIndex)
{
	Point2D<int> diff = getPositionFromId(id)-_worldPos;
	// left
	
	Rectangle<int> result;

	// origin
	if(diff._x==-1)
	{
		result._origin._x = 0;
	}
	else if(diff._x==0)
	{
		if(sectionIndex==0 || sectionIndex==2)
		{
			result._origin._x = 0;
		}
		else
		{
			result._origin._x = _boundaries._size._x/2;	
			if(_boundaries._origin._x+_boundaries._size._x!=_globalBoundaries._size._x)
			{
				result._origin._x -= _overlap;
			}
		}
	}
	else
	{
		result._origin._x = _boundaries._size._x - _overlap;
	}
	
	if(diff._y==-1)
	{
		result._origin._y = 0;
	}
	else if(diff._y==0)
	{
		if(sectionIndex==0 || sectionIndex==1)
		{
			result._origin._y = 0;
		}
		else
		{
			result._origin._y = _boundaries._size._y/2;
			if(_boundaries._origin._y+_boundaries._size._y!=_globalBoundaries._size._y)
			{
				result._origin._y -= _overlap;
			}
		}
	}
	else
	{
		result._origin._y = _boundaries._size._y - _overlap;
	}

	// size
	if(diff._x!=0)
	{
		result._size._x = _overlap*2;
	}
	else
	{
		// this result is = to boundaries + 1 overlap
		result._size._x = _boundaries._size._x/2;
		// border
		if(_boundaries._origin._x!=0)
		{
			result._size._x += _overlap;
		}
		else
		{
			if(sectionIndex!=0 && sectionIndex!=2)
			{
				result._size._x += _overlap;
			}
		}

		if(_boundaries._origin._x+_boundaries._size._x!=_globalBoundaries._size._x)
		{
			result._size._x += _overlap;
		}
		else
		{
			if(sectionIndex!=1 && sectionIndex!=3)
			{
				result._size._x += _overlap;
			}
		}
	}
	
	if(diff._y!=0)
	{
		result._size._y = _overlap*2;
	}
	else
	{
		result._size._y = _boundaries._size._y/2;
		if(_boundaries._origin._y!=0)
		{
			result._size._y += _overlap;
		}
		else
		{
			if(sectionIndex!=0 && sectionIndex!=1)
			{
				result._size._y += _overlap;
			}
		}

		if(_boundaries._origin._y+_boundaries._size._y!=_globalBoundaries._size._y)
		{
			result._size._y += _overlap;
		}
		else
		{
			if(sectionIndex!=2 && sectionIndex!=3)
			{
				result._size._y += _overlap;
			}
		}
	}
	//std::cout << "overlap between: " << _simulation.getId()  << " with world pos: " << _worldPos << " and neighbor: " << id << " and diff: " << diff << " in section index: " << sectionIndex << " is: " << result << std::endl;
	return result;
}

bool World::needsToBeUpdated( const int & id, const int & sectionIndex )
{	
	Point2D<int> diff = getPositionFromId(id)-_worldPos;
	switch(sectionIndex)
	{
		case 0:
			// top left
			if(diff._x==-1 && diff._y==-1)
			{
				return true;
			}
			// left
			if(diff._x==-1 && diff._y==0)
			{
				return true;
			}
			// top
			if(diff._x==0 && diff._y==-1)
			{
				return true;
			}
			return false;
			break;
		case 1:	
			// top
			if(diff._x==0 && diff._y==-1)
			{
				return true;
			}
			// top right
			if(diff._x==1 && diff._y==-1)
			{
				return true;
			}
			// right
			if(diff._x==1 && diff._y==0)
			{
				return true;
			}
			return false;
			break;
		case 2:	
			// bottom
			if(diff._x==0 && diff._y==1)
			{
				return true;
			}
			// bottom left
			if(diff._x==-1 && diff._y==1)
			{
				return true;
			}
			// left
			if(diff._x==-1 && diff._y==0)
			{
				return true;
			}
			return false;
			break;
		case 3:	
			// right
			if(diff._x==1 && diff._y==0)
			{
				return true;
			}
			// bottom right
			if(diff._x==1 && diff._y==1)
			{
				return true;
			}
			// bottom
			if(diff._x==0 && diff._y==1)
			{
				return true;
			}
			return false;
			break;
		default:
			std::stringstream oss;
			oss << "World::needsToBeUpdated - wrong section index: " << sectionIndex;
			throw Engine::Exception(oss.str());
	}
	return false;
}

bool World::needsToReceiveData( const int & id, const int & sectionIndex )
{	
	Point2D<int> diff = getPositionFromId(id)-_worldPos;
	switch(sectionIndex)
	{
		case 0:
			// bottom
			if(diff._x==0 && diff._y==1)
			{
				return true;
			}
			// right
			if(diff._x==1 && diff._y==0)
			{
				return true;
			}
			// bottom right
			if(diff._x==1 && diff._y==1)
			{
				return true;
			}
			return false;
			break;
		case 1:	
			// left
			if(diff._x==-1 && diff._y==0)
			{
				return true;
			}
			// bottom left
			if(diff._x==-1 && diff._y==1)
			{
				return true;
			}
			// bottom
			if(diff._x==0 && diff._y==1)
			{
				return true;
			}
			return false;
			break;
		case 2:	
			// top
			if(diff._x==0 && diff._y==-1)
			{
				return true;
			}
			// top right
			if(diff._x==1 && diff._y==-1)
			{
				return true;
			}
			// right
			if(diff._x==1 && diff._y==0)
			{
				return true;
			}
			return false;
			break;
		case 3:	
			// top left
			if(diff._x==-1 && diff._y==-1)
			{
				return true;
			}
			// left
			if(diff._x==-1 && diff._y==0)
			{
				return true;
			}
			// top
			if(diff._x==0 && diff._y==-1)
			{
				return true;
			}
			return false;
			break;
		default:
			std::stringstream oss;
			oss << "World::needsToReceiveData - wrong section index: " << sectionIndex;
			throw Engine::Exception(oss.str());
	}
	return false;
}

int World::getId()
{
	return _simulation.getId();
}

void World::removeAgent( Agent * agent )
{
	//std::cout << _simulation.getId() << " removing agent: " << agent << std::endl;
	AgentsList::iterator it = getOwnedAgent(agent->getId());
	if(it==_agents.end())
	{
		std::stringstream oss;
		oss << "World::removeAgent - agent: " << agent << " not found";
		throw Exception(oss.str());
	}
	// TODO it is not needed if it has modified position, as it is already done after the executed of a given agent step
	//sendDeleteOverlapAgent(it, agent->getPosition());
	_removedAgents.push_back(*it);
}

void World::removeAgents()
{
	//std::cout << _simulation.getId() << " execution agents removal: " << _removedAgents.size() << std::endl;
	AgentsList::iterator it=_removedAgents.begin();
	while(it!=_removedAgents.end())
	{
		Agent * agent = *it;
		//std::cout << "removing agent: " << agent << std::endl;
		AgentsList::iterator itAg = getOwnedAgent(agent->getId());
		if(itAg==_agents.end())
		{
			std::stringstream oss;
			oss << "World::removeAgents - agent: " << agent << " not found";
			throw Exception(oss.str());
			return;
		}
		_agents.erase(itAg);
		it++;
		//delete agent;
	}
	_removedAgents.clear();
}

void World::setSearchAgents( const bool & searchAgents )
{
	_searchAgents = searchAgents;
}

bool World::getSearchAgents()
{
	return _searchAgents;
}


int World::countNeighbours( Agent * target, const float & radius, const std::string & type )
{

	int numAgents = for_each(_agents.begin(), _agents.end(), aggregatorCount<Engine::Agent>(radius,*target, type))._count;
	int numOverlapAgents = for_each(_overlapAgents.begin(), _overlapAgents.end(), aggregatorCount<Engine::Agent>(radius,*target, type))._count;
	return numAgents+numOverlapAgents;
}

World::AgentsList World::getNeighbours( Agent * target, const float & radius, const std::string & type )
{
	AgentsList agentsList = for_each(_agents.begin(), _agents.end(), aggregatorGet<Engine::Agent>(radius,*target, type))._neighbors;
	AgentsList overlapAgentsList =  for_each(_overlapAgents.begin(), _overlapAgents.end(), aggregatorGet<Engine::Agent>(radius,*target, type))._neighbors;
	agentsList.merge(overlapAgentsList);
	return agentsList;
//	.merge(for_each(_overlapAgents.begin(), _overlapAgents.end(), aggregatorGet<Engine::Agent>(radius,*target, type))._neighbors);
	/*
	AgentsList neighbours;
	bool particularType = type.compare("all");
	if(_searchAgents)
	{
		for(AgentsList::iterator it=_agents.begin(); it!=_agents.end(); it++)
		{
			Agent * agent = (*it);
			if(agent!=target && agent->exists())
			{
				// we are not using distance method of Position, as this is not a Manhattan distance
				//int distanceX = std::abs(agent->getPosition()._x - target->getPosition()._x);
				//int distanceY = std::abs(agent->getPosition()._y - target->getPosition()._y);
				int distance = agent->getPosition().distance(target->getPosition());
				// sqrt(distanceX*distanceX + distanceY*distanceY);
				if(distance<(float)radius)
				{
					if(particularType)
					{	
						if(agent->isType(type))
						{
							neighbours.push_back(agent);
						}
					}
					else
					{
						neighbours.push_back(agent);
					}
				}			
			}
		}
		for(AgentsList::iterator it=_overlapAgents.begin(); it!=_overlapAgents.end(); it++)
		{
			Agent * agent = (*it);
			if(agent!=target && agent->exists())
			{
				// we are not using distance method of Position, as this is not a Manhattan distance
				int distanceX = std::abs(agent->getPosition()._x - target->getPosition()._x);
				int distanceY = std::abs(agent->getPosition()._y - target->getPosition()._y);
				int distance = sqrt(distanceX*distanceX + distanceY*distanceY);
				if(distance<(float)radius)
				{
					if(particularType)
					{	
						if(agent->isType(type))
						{
							neighbours.push_back(agent);
						}
					}
					else
					{
						neighbours.push_back(agent);
					}
				}	
			}
		}

	}
	else
	{
		Engine::Point2D<int> newPosition;
		for(newPosition._x=target->getPosition()._x-radius; newPosition._x<=target->getPosition()._x+radius; newPosition._x++)
		{
			for(newPosition._y=target->getPosition()._y-radius; newPosition._y<=target->getPosition()._y+radius; newPosition._y++)
			{
				Agent * agent = getAgent(newPosition);
				if(agent && agent!=target && agent->exists())
				{
					if(particularType)
					{
						if(agent->isType(type))
						{
							neighbours.push_back(agent);
						}
					}
					else
					{
						neighbours.push_back(agent);
					}
				}
			}
		}
	}
	return neighbours;
	*/
}

Serializer & World::getSerializer()
{
	return _serializer;
}

const int & World::getOverlap()
{
	return _overlap;
}

Point2D<int> World::getRandomPosition()
{
	while(1)
	{
		Engine::Point2D<int> pos(_statistics.getUniformDistValue(0,_boundaries._size._x-1), _statistics.getUniformDistValue(0,_boundaries._size._y-1));
		pos += _boundaries._origin;
		if(checkPosition(pos) && _boundaries.isInside(pos))
		{
			return pos;
		}
	}
}

bool World::rasterToSerialize( const std::string & key )
{
	std::list<std::string>::iterator it = std::find(_rastersToSerialize.begin(), _rastersToSerialize.end(), key);
	if(it!=_rastersToSerialize.end())
	{
		return true;
	}
	return false;
}

} // namespace Engine

