
#include "AgroPastoralist.hxx"
#include "GujaratWorld.hxx"
#include "CultivatedField.hxx"
#include "Exceptions.hxx"

#include "MoveHomeAction.hxx"
#include "AbandonPlotAction.hxx"
#include "StablishPlotAction.hxx"
#include "SowAction.hxx"
#include "MaintainPlotAction.hxx"
#include "HarvestAction.hxx"

namespace Gujarat
{

AgroPastoralist::AgroPastoralist( const std::string & id ) : GujaratAgent(id), _maxCropHomeDistance(10), _cultivatedField(0)
{
}

AgroPastoralist::~AgroPastoralist()
{
}

void AgroPastoralist::updateKnowledge()
{
}

void AgroPastoralist::evaluateYearlyActions()
{
	//std::cout << this << " evaluating yearly actions" << std::endl;	
	// 20% of chance
	if(_world->getStatistics().getUniformDistValue(0,4)==0)
	{
		_actions.push_back(new MoveHomeAction());
	}
	if(!_cultivatedField || _world->getValue("resourceType", _cultivatedField->getPosition())==FALLOW)
	{
		_actions.push_back(new AbandonPlotAction());
		_actions.push_back(new StablishPlotAction());
	}
}

void AgroPastoralist::evaluateSeasonalActions()
{
	GujaratWorld * world = (GujaratWorld*)_world;
	if(world->getClimate().getSeason()==COLDDRY && _world->getValue("resourceType", _cultivatedField->getPosition())==DOMESTICATED)
	{
		_actions.push_back(new SowAction());
		_actions.push_back(new MaintainPlotAction());
		_actions.push_back(new HarvestAction());
	}
}

void AgroPastoralist::evaluateIntraSeasonalActions()
{
}

void * AgroPastoralist::createPackage()
{
	return 0;
}

void AgroPastoralist::serializeAdditionalAttributes()
{
}
	
GujaratAgent * AgroPastoralist::createNewAgent()
{	
	GujaratWorld * world = (GujaratWorld*)_world;
	std::ostringstream oss;
	oss << "AgroPastoralist_" << world->getId() << "-" << world->getNewKey();
	return new AgroPastoralist(oss.str());
}

void AgroPastoralist::stablishPlot()
{
	// TODO by now all crops are of the same efficiency
	Engine::Point2D<int> newPosition(-1,-1);
	std::vector<Engine::Point2D<int> > possiblePositions;
	for(newPosition._x=_position._x-_maxCropHomeDistance; newPosition._x<=_position._x+_maxCropHomeDistance; newPosition._x++)
	{
		for(newPosition._y=_position._y-_maxCropHomeDistance; newPosition._y<=_position._y+_maxCropHomeDistance; newPosition._y++)
		{
			// by now common home is excluded
			if(_world->getOverlapBoundaries().isInside(newPosition) && _world->checkPosition(newPosition))
			{
				if(_world->getValue("soils", newPosition)==INTERDUNE && _world->getValue("resourceType", newPosition)==WILD)
				{
					
					possiblePositions.push_back(newPosition);
				}
			}
		}
	}
	// if there are no possible places, move home and stablish plot again
	if(possiblePositions.size()==0)
	{
		throw Engine::Exception("agent without crop");

		_actions.push_back(new MoveHomeAction());
		_actions.push_back(new StablishPlotAction());
		return;
	}
	std::random_shuffle(possiblePositions.begin(), possiblePositions.end());
	_cultivatedField = new CultivatedField((GujaratWorld&)(*_world), possiblePositions[0]);	
	//std::cout << this << " stablishing plot at position: " << _cultivatedField->getPosition() << std::endl;

	/*
	Engine::Point2D<int> newPosition(-1,-1);
	Engine::Point2D<int> bestPosition(-1,-1);
	for(newPosition._x=_position._x-_maxCropHomeDistance; newPosition._x<=_position._x+_maxCropHomeDistance; newPosition._x++)
	{
		for(newPosition._y=_position._y-_maxCropHomeDistance; newPosition._y<=_position._y+_maxCropHomeDistance; newPosition._y++)
		{
			// by now common home is excluded
			if(_world->getOverlapBoundaries().isInside(newPosition) && _world->checkPosition(newPosition))
			{
				if(_world->getValue("soils", newPosition)==INTERDUNE && _world->getValue("resourceType", newPosition)==WILD)
				{
					if(bestPosition._x==-1 || (_world->getValue("resources", newPosition)>_world->getValue("resources", bestPosition)))
					{
						bestPosition = newPosition;
					}
				}
			}
		}
	}
	_cultivatedField = new CultivatedField((GujaratWorld&)(*_world), bestPosition);	
	*/
}

void AgroPastoralist::sow()
{	
	if(_world->getValue("resourceType", _cultivatedField->getPosition())==DOMESTICATED && !_cultivatedField->isSown())
	{
		_cultivatedField->sow();
	}
}

void AgroPastoralist::maintainPlot()
{
	//std::cout << "agent maintains plot" << std::endl;
}

void AgroPastoralist::harvest()
{
	if(_cultivatedField->isSown())
	{
		_cultivatedField->harvest();
		_collectedResources += _world->getValue("resources", _cultivatedField->getPosition());
	}
}

void AgroPastoralist::abandonPlot()
{	
	if(_cultivatedField)
	{
		_world->setValue("resourceType", _cultivatedField->getPosition(), FALLOW);
		delete _cultivatedField;
	}
}

void AgroPastoralist::moveHome()
{	
	GujaratAgent::moveHome();
	if(_cultivatedField && _position.distance(_cultivatedField->getPosition())>_maxCropHomeDistance)
	{
		_actions.push_back(new AbandonPlotAction());
		_actions.push_back(new StablishPlotAction());
	}
}

} // namespace Gujarat

