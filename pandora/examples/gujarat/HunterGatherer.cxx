
#include "HunterGatherer.hxx"
#include "GujaratWorld.hxx"
#include "Exceptions.hxx"
//#include "Action.hxx"
#include "MoveHomeAction.hxx"
#include "GatherAction.hxx"
#include "HuntAction.hxx"

namespace Gujarat
{

HunterGatherer::HunterGatherer( const std::string & id ) : GujaratAgent(id), _surplusForReproductionThreshold(2), _surplusWanted(1)
{
}

HunterGatherer::~HunterGatherer()
{
}

void HunterGatherer::updateKnowledge()
{
}

void HunterGatherer::evaluateYearlyActions()
{
}

void HunterGatherer::evaluateSeasonalActions()
{
	// 100% of chance
	_actions.push_back(new MoveHomeAction());
}

void HunterGatherer::evaluateIntraSeasonalActions()
{
	std::list<Action*> actions;
	// TODO: which order must follow the actions? random?
	// now random

	// action pack : move, move Home, hunting, gathering

	int  planTime          = 0;
	int  failedAllocations = 0;
	bool actionMaskFails[] = {false, false, false};
	//int planEnergy = 0; // TODO just to remember someday we will estimate reward from a plan

	while(failedAllocations < 3 && planTime < _availableTime)
	// TODO may I do an estimation of reward received for each action?
	{
		int dice = _world->getStatistics().getUniformDistValue(1,10);
		if (dice != 2)
		{
			dice = _world->getStatistics().getUniformDistValue(0,1);
			// could avoid a call to random by doing dice=dice%2
			// but with more actions in the future... call random.
		}// else == moveHome
		Action *a = NULL;
		switch(dice)
		{
			case 0:
				a = new HuntAction();
				if (_availableTime < a->getTimeNeeded())
				{
					delete a;
					a = 0;
					if (!actionMaskFails[dice])
					{
						failedAllocations++;
						actionMaskFails[dice] = true;
					}
				}			
				break;
			case 1: 
				a = new GatherAction();
				if (_availableTime < a->getTimeNeeded())
				{
					delete a;
					a = 0;
					if (!actionMaskFails[dice])
					{
						failedAllocations++;
						actionMaskFails[dice] = true;
					}
				}
				break;
			case 2: 
				a = new MoveHomeAction();
				if (_availableTime < a->getTimeNeeded())
				{
					delete a;
					a = 0;
					if (!actionMaskFails[dice])
					{
						failedAllocations++;
						actionMaskFails[dice] = true;
					}
				}
				break;
		}
		if (a) 
		{ 
			_actions.push_back((Action*)a);
			planTime = planTime + a->getTimeNeeded();
		}
	}

}

void * HunterGatherer::createPackage()
{
	return 0;
}

void HunterGatherer::serializeAdditionalAttributes()
{
}

GujaratAgent * HunterGatherer::createNewAgent()
{	
	GujaratWorld * world = (GujaratWorld*)_world;
	std::ostringstream oss;
	oss << "HunterGatherer_" << world->getId() << "-" << world->getNewKey();
	return new HunterGatherer(oss.str());
}

void HunterGatherer::hunt()
{
	Engine::Point2D<int> location;
	if (_collectedResources < (_surplusForReproductionThreshold + _surplusWanted))
	{
	std::cout << "H";
		for(location._x=_position._x-GujaratAgent::_homeRange; location._x<=_position._x+GujaratAgent::_homeRange; location._x++)
		{
			for(location._y=_position._y-GujaratAgent::_homeRange; location._y<=_position._y+GujaratAgent::_homeRange; location._y++)
			{
				Agent * agent = _world->getAgent(location);
				if(_world->getOverlapBoundaries().isInside(location) && (!agent || !agent->exists() || agent==this))
				{
					int actualValue = _world->getValue("resources", location);
					//int collected = actualValue;
					// TODO watch your collect capacity / throughput 
					// depending on group size and your competence
					Soils soilType = (Soils) _world->getValue("soils", location);
					int type       = _world->getValue("resourceType", location);
					if (soilType == INTERDUNE && type == WILD && agent == 0)
					{
						int collected = _world->getStatistics().getUniformDistValue(0,actualValue); // NormalDistValue = competence?? 		
						_collectedResources += collected;
						_world->setValue("resources", location, actualValue-collected);
					}
				}
			}
		}
	std::cout << "***********************" << std::endl;
	}
}

void HunterGatherer::gather()
{
	Engine::Point2D<int> location;
	if (_collectedResources < (_surplusForReproductionThreshold + _surplusWanted))
	{
	std::cout << "G";
	for(location._x=_position._x-GujaratAgent::_homeRange; location._x<=_position._x+GujaratAgent::_homeRange; location._x++)
	{
		for(location._y=_position._y-GujaratAgent::_homeRange; location._y<=_position._y+GujaratAgent::_homeRange; location._y++)
		{
			Agent * agent = _world->getAgent(location);
			if(_world->getOverlapBoundaries().isInside(location) && (!agent || !agent->exists() || agent==this))
			{
				int actualValue = _world->getValue("resources", location);
				//int collected = actualValue;
				// TODO watch your collect capacity / throughput 
				// depending on group size and your competence
				Soils soilType = (Soils)_world->getValue("soils", location);
				int type       = _world->getValue("resourceType", location);
				if (soilType == INTERDUNE && type == WILD && agent == 0)
				{
					int collected = _world->getStatistics().getUniformDistValue(actualValue/5,actualValue/2); // NormalDistValue = competence??
					_collectedResources += collected;
					_world->setValue("resources", location, actualValue-collected);
				}
			}
		}
	}
	std::cout << "***********************" << std::endl;
	}
}
/*
void HunterGatherer::moveHome()
{
	Engine::Point2D<int> location;
	for(location._x=_position._x-GujaratAgent::_homeRange; location._x<=_position._x+GujaratAgent::_homeRange; location._x++)
	{
		for(location._y=_position._y-GujaratAgent::_homeRange; location._y<=_position._y+GujaratAgent::_homeRange; location._y++)
		{
			Agent * agent = _world->getAgent(location);
			if(_world->getOverlapBoundaries().isInside(location) && (!agent || !agent->exists() || agent==this))
			{
				Soils soilType = (Soils)_world->getValue("soils", location);
				int type       = _world->getValue("resourceType", location);
				if (soilType == DUNE && type == WILD && agent == 0)
				{
					_world->occupy(location);
					_location = location;
				}
			}
		}
	}
}
*/



} // namespace Gujarat

