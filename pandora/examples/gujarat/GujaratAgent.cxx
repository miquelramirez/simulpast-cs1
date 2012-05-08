
#include "GujaratAgent.hxx"
#include "GujaratWorld.hxx"
#include "GujaratDemographics.hxx"
#include "Action.hxx"
#include "CaloricRequirementsTable.hxx"
#include <sstream>

namespace Gujarat
{

GujaratAgent::GujaratAgent( const std::string & id ) 
	: Engine::Agent(id), 
	 _spentTime(0), _collectedResources(0), _age(0),
	_socialRange( 50 ), _starved( false ), _caloricRequirements(0)
{
	// we start with a couple of 15 years
	_populationAges.push_back(15);
	_populationAges.push_back(15);
	_emigrationProbability = 0.0;
	_reproductionProbability = 0.0;
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

void GujaratAgent::updateKnowledge()
{
}

void GujaratAgent::step()
{
	logAgentState();
	updateKnowledge();
	selectActions();
	executeActions();
	updateState();
}

void GujaratAgent::logAgentState()
{
	log() << "timestep=" << getWorld()->getCurrentTimeStep() << std::endl;
	log() << "\tagent.collectedResourcesBeforeAction=" << getOnHandResources() << std::endl;	
	log() << "\tagent.nrAdults=" << getNrAvailableAdults() << std::endl;
	log() << "\tagent.adultAges=[";
	for ( unsigned k = 0; k < _populationAges.size(); k++ )
		if ( _populationAges[k] >= 15 )
			log() << _populationAges[k] << ",";
	log() << "]" << std::endl; 
	log() << "\tagent.nrChildren=" << getNrChildren() << std::endl;
	log() << "\tagent.childrenAges=[";
	for ( unsigned k = 0; k < _populationAges.size(); k++ )
		if ( _populationAges[k] >= 0 && _populationAges[k] < 15 )
			log() << _populationAges[k] << ",";	
	log() << "]" << std::endl;
	log() << "\tagent.resourcesNeeded=" << computeConsumedResources(1) << std::endl; 
}

void GujaratAgent::updateState()
{
	log() << "\tagent.collectedResourcesAfterAction=" << getOnHandResources() << std::endl;	
	_collectedResources -= computeConsumedResources(1);
	log() << "\tagent.collectedResourcesAfterConsumption=" << getOnHandResources() << std::endl;	
	if ( _collectedResources < 0 )
	{
		_starved = true;
		_emigrationProbability += 1.0f/120.f;
		log() << "\tagent.isStarving=yes" << std::endl;
		_collectedResources = 0;
	}
	else
	{
		log() << "\tagent.isStarving=no" << std::endl;
		_starved = false;
		_reproductionProbability += 1.0/360.0f;
		// Decay factor, modeling spoilage
		_collectedResources *= getSurplusSpoilageFactor();

	}

	if ( (getWorld()->getCurrentTimeStep() % 120 == 0) 
		&& (getWorld()->getCurrentTimeStep() > 119) ) // last day of the year
	{
		if( checkEmigration() )
		{
			log() << "\tagent.emigration=yes" << std::endl;
			_world->removeAgent(this);
			return;
		}
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

bool	GujaratAgent::checkEmigration()
{
	return _demographicsModel->checkEmigration();

}

double 	GujaratAgent::getTimeSpentForagingTile() const
{
	return getForageTimeCost() * getWalkingSpeedHour();
}

double	GujaratAgent::computeMaxForagingDistance() const
{
	int 	nAdults = getNrAvailableAdults();		

	return  getWalkingSpeedHour() * getAvailableForageTime() * (double)nAdults;
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
	_demographicsModel->checkReproduction();
}

int GujaratAgent::computeConsumedResources( int timeSteps ) const
{
	int requiredResources = 0;
	for(unsigned int index=0; index<_populationAges.size(); index++)
	{
		if(_populationAges[index]!=-1)
			requiredResources += _caloricRequirements->getCaloriesFor(_populationAges[index]);
	}
	return requiredResources * timeSteps;	
}

void GujaratAgent::checkMortality()
{
	_demographicsModel->checkMortality();
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

void	GujaratAgent::initializePosition( )
{
	// 1. select settlement area
	GujaratWorld* world = dynamic_cast<GujaratWorld*>(getWorld());
	const  std::vector< Engine::Rectangle<int> >& areas = world->getSettlementAreas()->getAreas();
	unsigned die = getWorld()->getStatistics().getUniformDistValue(0, areas.size()-1);
	Engine::Rectangle<int> area = areas[die];
	std::vector< Engine::Point2D<int> > dunes;
	for ( int x = area._origin._x; x < area._origin._x + area._size._x; x++ )
		for ( int y = area._origin._y; y < area._origin._y + area._size._y; y++ )
		{
			Engine::Point2D<int> p(x,y);
			if ( getWorld()->getValue("soils", p ) == DUNE )
				dunes.push_back( p );
		}
	assert( !dunes.empty() );
	die = getWorld()->getStatistics().getUniformDistValue(0, dunes.size()-1);
	setPosition( dunes[die] );
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

int	GujaratAgent::getPopulationSize() const
{
	int popSize = 0;
	for(unsigned int index=0; index<_populationAges.size(); index++)
	{
		if(_populationAges[index]!=-1)
		{
			popSize++;
		}
	}
	return popSize;	
}

void	GujaratAgent::decimatePopulation()
{
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

void	GujaratAgent::checkDeath( int minAge, int maxAge, int chance )
{
	for ( unsigned index = 0; index < _populationAges.size(); index++ )
	{
		if ( _populationAges[index] < 0 ) continue;
		if ( _populationAges[index] < minAge
			|| _populationAges[index] >= maxAge )
			continue;
		int die = _world->getStatistics().getUniformDistValue(0,1000);
		if ( die < chance )
			_populationAges[index] = -1;
	}
}

void	GujaratAgent::checkDeathByAging( int minAge )
{
	for ( unsigned index = 0; index < _populationAges.size(); index++ )
	{
		if ( _populationAges[index] <= minAge ) continue;
		int chance = _populationAges[index] - minAge;
		int die = _world->getStatistics().getUniformDistValue(0,100);
		if ( die < chance )
			_populationAges[index] = -1;
	}
	
}

bool	GujaratAgent::canReproduce() const
{
	if(_populationAges[0] == -1 || _populationAges[1] == -1)
	{
		return false;
	}
	return true;
}

void	GujaratAgent::addNewChild()
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

} // namespace Gujarat

