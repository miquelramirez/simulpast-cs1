#include "HunterGathererDecisionTreeController.hxx"
#include <vector>
#include <iostream>
#include "MoveHomeAction.hxx"
#include "ForageAction.hxx"
#include "DoNothingAction.hxx"
#include "HunterGatherer.hxx"
#include "SettlementAreas.hxx"
#include "GujaratWorld.hxx"
#include "Sector.hxx"

namespace Gujarat
{

HunterGathererDecisionTreeController::HunterGathererDecisionTreeController( GujaratAgent* a )
	: AgentController( a )
{
	_agentConcrete = dynamic_cast<HunterGatherer*>( a );
	_DoNothingDaysCovered = 1; 
}

HunterGathererDecisionTreeController::~HunterGathererDecisionTreeController()
{
}


Sector* HunterGathererDecisionTreeController::getMaxBiomassSector(  )
{
	// Make Forage actions
	std::vector< Sector* > validActionSectors;
	std::vector< Sector* > actionSectors;

	_agentConcrete->updateKnowledge( agentRef().getPosition(), agentRef().getWorld()->getDynamicRaster( "resources" ), actionSectors );

	// MRJ: Remove empty sectors if any
	for ( unsigned i = 0; i < actionSectors.size(); i++ )
	{
		if ( actionSectors[i]->isEmpty() )
		{
			delete actionSectors[i];
			continue;
		}
		validActionSectors.push_back( actionSectors[i] );
	}	
	
	if( validActionSectors.empty() ) return NULL;

	// Find Sector with Maximum Biomass to Forage 
	int maxBiomass = validActionSectors[0]->getBiomassAmount();
	unsigned maxBiomassIdx = 0;
	for ( unsigned i = 1; i < validActionSectors.size(); i++ )
	{
		if( validActionSectors[i]->getBiomassAmount() > maxBiomass )
		{
			maxBiomass = validActionSectors[i]->getBiomassAmount();
			maxBiomassIdx = i;
		}
	}

	for ( unsigned i = 0; i < validActionSectors.size(); i++ )
		if( i != maxBiomassIdx)
			delete validActionSectors[i];

	return validActionSectors[ maxBiomassIdx ];

}

Action* HunterGathererDecisionTreeController::shouldDoNothing(  )
{	
	// CollectedResources > ConsumedResourcesByAgent * #days
	if( agentRef().getOnHandResources() > agentRef().computeConsumedResources( getDoNothingDaysCovered() ) )
	{
		return new DoNothingAction();
	}
	else
	{
		return NULL;
	}
}

Action* HunterGathererDecisionTreeController::shouldForage(  )
{	
	Sector* maxSector = getMaxBiomassSector();

	if( maxSector == NULL) 
		return NULL;
	
	Action* forage = NULL;
	if( maxSector->getBiomassAmount() >=  agentRef().computeConsumedResources(1) )
		forage = new ForageAction( maxSector, true );

	return forage;
	

}

Action* HunterGathererDecisionTreeController::shouldMoveHome(  )
{
	const Engine::Point2D<int>& agentPos = agentRef().getPosition();
	
	// Put a BOX around the home range.	
	int boxOriginX = agentPos._x - agentRef().getHomeMobilityRange();
	int boxOriginY = agentPos._y - agentRef().getHomeMobilityRange();
	int boxSizeX = 2*agentRef().getHomeMobilityRange()+1;
	int boxSizeY = 2*agentRef().getHomeMobilityRange()+1;
	Engine::Point2D<int> boxOrigin(boxOriginX, boxOriginY);	
	Engine::Point2D<int> boxSize(boxSizeX,boxSizeY);	
	Engine::Rectangle<int> unTrimmedHomeBox(boxOrigin,boxSize);
	Engine::Rectangle<int> homeBox;
	//TODO look out sectors, MPI regions, etc... Decide getBoundaries? or getOverlapBoundaries?
	// MRJ: As I understand it, this should be getOverlapBoundaries(), very much as it was before
	unTrimmedHomeBox.intersection(agentRef().getWorld()->getOverlapBoundaries(),homeBox);
	
	// Retrieve the areas that have intersection non zero with homeBox
	const GujaratWorld * world              = (GujaratWorld *)agentRef().getWorld();
	const SettlementAreas * settlementAreas = world->getSettlementAreas();
	std::vector<int> candidates(0);
	settlementAreas->intersectionFilter(homeBox,candidates);

	assert( !candidates.empty() );
	
	// Select Areas with maximum score.
	// Sort candidates following scores	
	std::make_heap(candidates.begin(),candidates.end(),Gujarat::compareSettlementAreas(settlementAreas));
	
	int i = 0;
	Action* moveHome = NULL;
	Engine::Point2D<int> newHomeLocation;
	
	assert( !candidates.empty() );
	Engine::Rectangle<int> selectedArea = settlementAreas->getAreaById(candidates[i]);
	Engine::Rectangle<int> intersection;
	// MRJ: If the selected area is outside of the "homeBox" then why caring about it all?
	// ATM: above you will find settlementAreas->intersectionFilter method call. This ensures any
	// item inside candidates vector has intersection with homeBox. So the 'if' is not
	// needed, we just recompute the intersection to use it.
	homeBox.intersection(selectedArea, intersection);
		
	// Extract one random dune cell which is inside the homeRange and inside the selected area.
		
	// count dunes from candidate area "i", 'selectedArea' variable
	int countDunes = 0;
	Engine::Point2D<int> index;
	std::vector< Engine::Point2D<int> > dunes;
	for (index._x = intersection._origin._x; index._x < intersection._origin._x+intersection._size._x; index._x++)			
	{
		for (index._y = intersection._origin._y; index._y < intersection._origin._y+intersection._size._y; index._y++)			
		{
			if ((world->getValue("soils",index) == DUNE) 
			    && (ceil(agentPos.distance(index)) <= (double)agentRef().getHomeMobilityRange()))
			{
				countDunes++;
				dunes.push_back(index);
			}
		}
	}		

	if ( dunes.empty() ) 
	{
		return NULL;
	}
	uint32_t diceSelectOneRandomDune = world->getStatistics().getUniformDistValue(0, dunes.size()-1);
	newHomeLocation = dunes[ diceSelectOneRandomDune ];
	moveHome = new MoveHomeAction( dunes[ diceSelectOneRandomDune ] );

	assert( moveHome != NULL );
	candidates.clear();


	//Check if new SettlementArea have enough biomass to forage for one day
	//Engine::Point2D<int> oldHomeLocation = agentRef().getPosition();
	//agentRef().setPosition( newHomeLocation );
	
	//Sector* maxSector = getMaxBiomassSector();
	
	//Not sure is a good option, it makes the agent too conservative 
	//without exploring further settlementAreas

	//if( maxSector->getBiomassAmount() >= agentRef().computeConsumedResources(1) )
	//{	
	//	agentRef().setPosition( oldHomeLocation );
	//	delete maxSector;
		return moveHome;
	//}
	
	//delete moveHome;	
	//return NULL;
	



}

Action*	HunterGathererDecisionTreeController::selectAction()
{

        //Decission Tree: DoNothing --> Forage --> MoveHome

	Action* selectedAction = NULL;
	
	selectedAction = shouldDoNothing();	
	selectedAction = (!selectedAction) ?  shouldForage() : selectedAction;
	selectedAction = (!selectedAction) ?  shouldMoveHome() : selectedAction;

	//DefaultAction if non is selected
	selectedAction = (!selectedAction) ?  new DoNothingAction() : selectedAction;

	return selectedAction;
	
}

}
