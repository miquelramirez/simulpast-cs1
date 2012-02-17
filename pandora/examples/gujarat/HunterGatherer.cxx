
#include "HunterGatherer.hxx"
#include "GujaratWorld.hxx"
#include "Exceptions.hxx"
//#include "Action.hxx"
#include "MoveHomeAction.hxx"
#include "GatherAction.hxx"
#include "HuntAction.hxx"

namespace Gujarat
{

HunterGatherer::HunterGatherer( const std::string & id ) 
	: GujaratAgent(id), _surplusForReproductionThreshold(2), _surplusWanted(1), _homeRange(50)
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
}

void HunterGatherer::evaluateIntraSeasonalActions()
{
	// TODO: which order must follow the actions? random?
	// now random

	// action pack : move Home, hunting, gathering
	int dice = _world->getStatistics().getUniformDistValue(1,10);

	if ( dice >= 8 ) // p=0.2 agent chooses to move its home
	{
		_actions.push_back( new MoveHomeAction() );
		return;
	}

	if ( dice < 4 ) // select Hunt
	{
		_actions.push_back( new HuntAction() );
		return;
	}

	_actions.push_back( new GatherAction() );

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

bool	HunterGatherer::needsResources()
{
	return _collectedResources < (_surplusForReproductionThreshold + _surplusWanted);
}

bool	HunterGatherer::cellValid( Engine::Point2D<int>& loc )
{
	if ( !_world->getOverlapBoundaries().isInside(loc) )
		return false;
	// Check that the home of another agent resides in loc
	Agent * agent = _world->getAgent(loc);
	if ( agent && agent->exists() && (agent != this ) )
		return false;
	return true;
}

bool	HunterGatherer::cellRelevant( Engine::Point2D<int>& loc )
{
	Soils soilType = (Soils) _world->getValue("soils", loc);
	int resourceType = _world->getValue("resourceType", loc);
	return soilType == INTERDUNE && resourceType == WILD;
}

} // namespace Gujarat

