
#include "GujaratAgent.hxx"
#include "GujaratWorld.hxx"
#include "Action.hxx"
#include <sstream>

namespace Gujarat
{

GujaratAgent::GujaratAgent( const std::string & id ) 
	: Engine::Agent(id), 
	 _spentTime(0), _collectedResources(0), _age(0),
	_socialRange( 50 ), _starvated( false )
{
	// we start with a couple of 15 years
	_populationAges.push_back(15);
	_populationAges.push_back(15);
	std::stringstream fName;
	fName << getId() << ".state.log";
	_log = new std::ofstream( fName.str().c_str() );
}

GujaratAgent::~GujaratAgent()
{
	_log->close();
	delete _log;
}

void	GujaratAgent::setController(  AgentController* controller )
{
	_controller = controller;
}

int	GujaratAgent::convertBiomassToCalories( int biomass ) const
{
	float fMass = (float)biomass;
	return (int)(fMass* _massToCaloriesRate);	
}

int	GujaratAgent::computeEffectiveBiomassForaged( int nominal ) const
{
	return _world->getStatistics().getNormalDistValue(0, nominal);
	
}

void GujaratAgent::setAvailableTime( int daysPerSeason )
{
	_availableTime = 120 / daysPerSeason;
}

void GujaratAgent::step()
{
	log() << "timestep=" << getWorld()->getCurrentTimeStep() << std::endl;
	log() << "\tagent.collectedResourcesBeforeAction=" << getOnHandResources() << std::endl;	

	updateKnowledge();

	GujaratWorld * world = (GujaratWorld*)_world;
	// rain season is the first of the year; evaluate year actions
	if(world->getClimate().getSeason()==HOTWET)
	{
		evaluateYearlyActions();
		executeActions();
	}
	evaluateSeasonalActions();
	executeActions();

	evaluateIntraSeasonalActions();
	executeActions();

	_spentTime = 0;

	// age of the agent, in seasons
	_age++;
	log() << "\tagent.collectedResourcesAfterAction=" << getOnHandResources() << std::endl;	
	_collectedResources -= computeConsumedResources(1);
	log() << "\tagent.collectedResourcesAfterConsumption=" << getOnHandResources() << std::endl;	
	if ( _collectedResources < 0 )
	{
		_starvated = true;
		log() << "\tagent.isStarvating=yes" << std::endl;
		_collectedResources = 0;
	}
	else
	{
		log() << "\tagent.isStarvating=no" << std::endl;
		_starvated = false;
		// Decay factor, modeling spoilage
		_collectedResources *= getSurplusSpoilageFactor();

	}
	
	// end of year, evaluate reproduction, mortality and update age
	if( (getWorld()->getCurrentTimeStep() % 360 == 0) 
		&& (getWorld()->getCurrentTimeStep() > 359) ) // last day of the year
	{
		updateAges();
		checkMortality();
		checkReproduction();
		checkMarriage();
		checkAgentRemoval();
	}
}

double 	GujaratAgent::getForageTime() const
{
	double forageTime = 0.5;
	double walkingSpeedHour = 3000.0;
	return forageTime * walkingSpeedHour;
}

double	GujaratAgent::computeMaxForagingDistance() const
{
	int 	nAdults = getNrAvailableAdults();		
	double  walkingSpeedHour = 3000.0;
	double  availTime = 4.5;
	double  distPerAdult = walkingSpeedHour * availTime;

	return  distPerAdult * (double)nAdults;
}

GujaratAgent * GujaratAgent::getMarriageCandidate()
{
	std::vector<GujaratAgent*> possible;
	Engine::World::AgentsList listOfNeighbors = _world->getNeighbours(this, _socialRange, getType());
	for(Engine::World::AgentsList::iterator it=listOfNeighbors.begin(); it!=listOfNeighbors.end(); it++)
	{
		GujaratAgent * possibleAgent = (GujaratAgent*)(*it);
		for(unsigned i=2; i<possibleAgent->_populationAges.size(); i++)
		{	
			// it avoids dead children (-1)
			if(possibleAgent->_populationAges[i]>15)
			{
				possible.push_back(possibleAgent);
			}
		}
	}

	if(possible.size()==0)
	{
		return 0;
	}
	std::random_shuffle(possible.begin(), possible.end());
	return possible[0];
}

void GujaratAgent::checkMarriage()
{
	for(unsigned i=2; i<_populationAges.size(); i++)
	{
		if(_populationAges[i]>15)
		{
			GujaratAgent * agent = getMarriageCandidate();

			if(!agent)
			{
				//std::cout << "no agent near this one with candidate" << std::endl;
				return;
			}
			
			// TODO male female??
			GujaratAgent * newAgent = agent->createNewAgent();

			newAgent->_populationAges[0] = _populationAges[i];
			_populationAges[i] = -1;

			for(unsigned int j=2; j<agent->_populationAges.size(); j++)
			{
				if(agent->_populationAges[j]>15)
				{
					newAgent->_populationAges[1] = agent->_populationAges[j];
					agent->_populationAges[j] = -1;
					break;
				}
			}
			// location inside home range of husband family
			newAgent->setPosition(getNearLocation(getSocialRange()));
			_world->addAgent(newAgent);
			//std::cout << "new agent created: " << newAgent << " with husband age: " << newAgent->_populationAges[0] << " and wife age: " << newAgent->_populationAges[1] << std::endl;
		}
	}
}

Engine::Point2D<int> GujaratAgent::getNearLocation( int range )
{
	std::vector<Engine::Point2D<int> > possiblePositions;
	// TODO this method excludes the creation of children in cells in other environments
	Engine::Point2D<int> location = _position;
	for(location._x=_position._x-range; location._x<=_position._x+range; location._x++)
	{
		for(location._y=_position._y-range; location._y<=_position._y+range; location._y++)
		{
			if(	_world->getOverlapBoundaries().isInside(location) && 
				_world->checkPosition(location) && _world->getValue("soils", location)==DUNE)
			{
				possiblePositions.push_back(location);
			}
		}
	}

	// no one near the agent
	if(possiblePositions.size()==0)
	{
		return Engine::Point2D<int>(-1,-1);
	}
	std::random_shuffle(possiblePositions.begin(), possiblePositions.end());
	return possiblePositions[0];
}

void GujaratAgent::executeActions()
{
	std::list<Action *>::iterator it = _actions.begin();
	unsigned i = 0;
	while(it!=_actions.end())
	{
		Action * nextAction = *it;
		//_spentTime += nextAction->getTimeNeeded();
		//if(_spentTime<=_availableTime)
		//{
		nextAction->execute(*this);
		log() << "\tagent.action[" << i << "]=";
		nextAction->describe(log());
		log() << std::endl;
		//}
		it = _actions.erase(it);
		delete nextAction;
		i++;
	}
	
}

void GujaratAgent::checkAgentRemoval()
{
	if(_populationAges[0] == -1 && _populationAges[1] == -1)
	{
		//std::cout << "agent: " << this << " removed" << std::endl;
		_exists = false;
		_world->removeAgent(this);
	}
}

void GujaratAgent::updateAges()
{
	for(unsigned int i=0; i<_populationAges.size(); i++)
	{
		if(_populationAges[i]!=-1)
		{
			_populationAges[i]++;
		}
	}
}

void GujaratAgent::checkReproduction()
{
	// if male or female died, reproduction is impossible
	if(_populationAges[0] == -1 || _populationAges[1] == -1)
	{
		return;
	}
	// 50% chance of having new child
	if(_world->getStatistics().getUniformDistValue(0,1)==0)
	{
		//std::cout << this << " have a new baby!" << std::endl;
		// we look for an empty space or add a new one
		for(unsigned int index=2; index!=_populationAges.size(); index++)
		{
			if(_populationAges[index]==-1)
			{
				_populationAges[index] = 0;
				return;
			}
		}
		_populationAges.push_back(0);
	}
}

int GujaratAgent::computeConsumedResources( int timeSteps ) const
{
	int popSize = 0;
	for(unsigned int index=0; index<_populationAges.size(); index++)
	{
		if(_populationAges[index]!=-1)
		{
			popSize++;
		}
	}
	return 2000.0f * popSize;	
}

void GujaratAgent::checkMortality()
{
	// TODO check resources, now year, must be seasonal
	int popSize = 0;
	for(unsigned int index=0; index<_populationAges.size(); index++)
	{
		if(_populationAges[index]!=-1)
		{
			popSize++;
		}
	}
	// each individual eats 10 resources
	int maintainedPopulation = _collectedResources/2000.0f;
	int starvingPopulation = popSize - maintainedPopulation;
	// for each starving pop, possibility of death = 10% for each individual
	if(starvingPopulation>0)
	{
		//std::cout << "starving pop!: " << starvingPopulation << " with collected resources: " << _collectedResources << " and pop size: " << popSize << std::endl;
		for(unsigned int index=0; index<_populationAges.size(); index++)
		{
			if(_populationAges[index]!=-1)
			{	
				if(_world->getStatistics().getUniformDistValue(0,9)==0)
				{
					_populationAges[index]=-1;
				}
			}
		}
	}

	for(unsigned int index=0; index<2; index++)
	{
		if(_populationAges[index]==-1)
		{
			continue;
		}
		// 2% chance of mortality in adults
		if(_world->getStatistics().getUniformDistValue(0,1000)<_populationAges[index])
		{
			//std::cout << "adult: " << index << " died at age: " << _populationAges[index] << std::endl;
			_populationAges[index] = -1;
		}
		/*
		// absolute maximum: 50 years
		if(_populationAges[index]==50)
		{
			//std::cout << "adult: " << index << " died at maximum age: " << std::endl;
			_populationAges[index] = -1;
		}
		*/
	}
	for(unsigned int index=2; index<_populationAges.size(); index++)
	{
		if(_populationAges[index]==-1)
		{
			continue;
		}
		// 10% chance of mortality until 2
		if(_populationAges[index]<3)
		{
			if(_world->getStatistics().getUniformDistValue(0,9)==0)
			{
				//std::cout << "child: " << _populationAges[index] << " died during infancy" << std::endl;
				_populationAges[index] = -1;
			}
		}
		// 2% afterwards (as male/female)
		//else if(_world->getStatistics().getUniformDistValue(0,24)==0)
		else if(_world->getStatistics().getUniformDistValue(0,1000)<_populationAges[index])
		{
			//std::cout << "child: " << _populationAges[index] << " died" << std::endl;
			_populationAges[index] = -1;
		}
	}
}

void GujaratAgent::serialize()
{
	serializeAttribute("agent age", _age);

	if(_populationAges[0]!=-1)
	{
		serializeAttribute("male alive", 1);
		serializeAttribute("male age", _populationAges[0]);
	}
	else
	{
		serializeAttribute("male alive", 0);
		serializeAttribute("male age", std::numeric_limits<int>::max());
	}
	
	if(_populationAges[1]!=-1)
	{
		serializeAttribute("female alive", 1);
		serializeAttribute("female age", _populationAges[1]);
	}
	else
	{
		serializeAttribute("female alive", 0);
		serializeAttribute("female age", std::numeric_limits<int>::max());
	}

	int numChildren = 0;
	for(unsigned i=2; i<_populationAges.size(); i++)
	{
		if(_populationAges[i]!=-1)
		{
			numChildren++;
		}
	}
	serializeAttribute("children", numChildren);
	serializeAttribute("collected resources", _collectedResources);
}

void	GujaratAgent::initializePosition( Engine::Point2D<int> randomPos )
{
	setPosition(randomPos);
	setPosition(getNearLocation(50));
}

int	GujaratAgent::getNrAvailableAdults() const
{
	int numAdults = 0;
	for(unsigned i=0; i<_populationAges.size(); i++)
	{
		if(_populationAges[i] >= 15)
		{
			numAdults++;
		}
	}
	return numAdults;
}

int	GujaratAgent::getNrChildren() const
{
	int numChildren = 0;
	for(unsigned i=0; i<_populationAges.size(); i++)
	{
		if(_populationAges[i] < 15)
		{
			numChildren++;
		}
	}
	return numChildren;
}

} // namespace Gujarat

