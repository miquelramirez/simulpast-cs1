
#include "AgroPastoralist.hxx"
#include "GujaratWorld.hxx"
#include "CultivatedField.hxx"
#include "Exceptions.hxx"

#include "MoveHomeAction.hxx"
#include "AbandonPlotAction.hxx"
#include "EstablishPlotAction.hxx"
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
	// MRJ: Commenting out until fixed
	
	if(_world->getStatistics().getUniformDistValue(0,4)==0)
	{
		std::vector< MoveHomeAction* > possibleActions;
		MoveHomeAction::generatePossibleActions( *this, possibleActions );
		
		int dice = _world->getStatistics().getUniformDistValue( 0, possibleActions.size() - 1 );

		MoveHomeAction* selectedAction = possibleActions[dice];
		possibleActions[dice] = NULL;
		_actions.push_back( selectedAction );
		for ( unsigned i = 0; i < possibleActions.size(); i++ )
			if ( possibleActions[i] != NULL )
				delete possibleActions[i];

		return;
	}
	if(_cultivatedField == NULL || _cultivatedField->requiresFallow() )
	{
		std::vector< EstablishPlotAction* > possibleEPActions;
		EstablishPlotAction::generatePossibleActions( *this, possibleEPActions );

		if ( possibleEPActions.empty() )
		{
			std::vector< MoveHomeAction* > possibleMHActions;
			MoveHomeAction::generatePossibleActions( *this, possibleMHActions );
			
			int dice = _world->getStatistics().getUniformDistValue( 0, possibleMHActions.size() - 1 );
	
			MoveHomeAction* selectedAction = possibleMHActions[dice];
			possibleMHActions[dice] = NULL;
			_actions.push_back( selectedAction );
			for ( unsigned i = 0; i < possibleMHActions.size(); i++ )
				if ( possibleMHActions[i] != NULL )
					delete possibleMHActions[i];
			return;
		}


		int dice = _world->getStatistics().getUniformDistValue( 0, possibleEPActions.size() - 1 );
		
		EstablishPlotAction* selectedAction = possibleEPActions[dice];
		possibleEPActions[dice] = NULL;
		_actions.push_back( selectedAction );
		for ( unsigned i = 0; i < possibleEPActions.size(); i++ )
			if ( possibleEPActions[i] != NULL )
				delete possibleEPActions[i];

	}
}

void AgroPastoralist::evaluateSeasonalActions()
{
	// MRJ: This is a HORRIBLE thing to do!!!
	GujaratWorld* world = dynamic_cast<GujaratWorld*>( _world );
	if( world->isColdDrySeason() && _cultivatedField->isDomesticated() )
	{
		_actions.push_back(new SowAction());
		_actions.push_back(new MaintainPlotAction());
		_actions.push_back(new HarvestAction());
	}

}

bool AgroPastoralist::cultivatedFieldOutOfReach()
{
	return _position.distance(_cultivatedField->getPosition()) > getMaxCropHomeDistance();
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

void AgroPastoralist::abandonCultivatedField()
{
	_world->setValue("resourceType", _cultivatedField->getPosition(), FALLOW);
	delete _cultivatedField;
}

void AgroPastoralist::acquireCultivatedField( Engine::Point2D<int> p )
{
	if ( _cultivatedField != NULL )
		abandonCultivatedField();
	_cultivatedField = new CultivatedField( (GujaratWorld&)(*_world), p );
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
	std::cout << "agent maintains plot" << std::endl;
}

void AgroPastoralist::harvest()
{
	if(_cultivatedField->isSown())
	{
		_cultivatedField->harvest();
		_collectedResources += _world->getValue("resources", _cultivatedField->getPosition());
	}
}

} // namespace Gujarat

