
#include "GujaratWorld.hxx"
#include "MoveHomeAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"
#include <algorithm>
#include <vector>

#include <stdint.h>


namespace Gujarat
{

MoveHomeAction::MoveHomeAction( Engine::Point2D<int>& p ) 
	: _newHomeLoc( p )
{
}

MoveHomeAction::~MoveHomeAction()
{
}

void MoveHomeAction::generatePossibleActions( 	GujaratAgent& agent,
						std::vector< MoveHomeAction* >& actions )
{
	// TODO this method excludes the creation of children in cells in other environments
	Engine::Point2D<int> location;
	Engine::Point2D<int> currPos = agent.getPosition();
	int range = agent.getHomeMobilityRange();

	for( location._x = currPos._x-range; 
		location._x <= currPos._x+range; location._x++)
	{
		for( location._y = currPos._y-range; 
			location._y <= currPos._y+range; location._y++)
		{
			if(	agent.getWorld()->getOverlapBoundaries().isInside(location) && 
				agent.getWorld()->checkPosition(location) && 
				agent.getWorld()->getValue("soils", location)==DUNE)
			{
				actions.push_back( new MoveHomeAction(location) );
			}
		}
	}
}

void MoveHomeAction::execute( GujaratAgent & agent )
{
	std::cout << "DEBUG: MoveHome action executing..." << std::endl;
	int prevHomeActivity = agent.getWorld()->getValue( "homeActivity", _newHomeLoc );
	
	// Put a BOX around the home range.	
	int boxOriginX = agent.getPosition()._x - agent.getHomeMobilityRange();
	int boxOriginY = agent.getPosition()._y - agent.getHomeMobilityRange();
	int boxSizeX = 2*agent.getHomeMobilityRange()+1;
	int boxSizeY = 2*agent.getHomeMobilityRange()+1;
	Engine::Point2D<int> boxOrigin(boxOriginX, boxOriginY);	
	Engine::Point2D<int> boxSize(boxSizeX,boxSizeY);	
	Engine::Rectangle<int> unTrimmedHomeBox(boxOrigin,boxSize); 
	Engine::Rectangle<int> homeBox;
	//TODO look out sectors, MPI regions, etc... Decide getBoundaries? or getOverlapBoundaries?
	// MRJ: As I understand it, this should be getOverlapBoundaries(), very much as it was before
	unTrimmedHomeBox.intersection(agent.getWorld()->getOverlapBoundaries(),homeBox);
	
	// Retrieve the areas that have intersection non zero with homeBox
	GujaratWorld * world              = (GujaratWorld *)agent.getWorld();
	SettlementAreas * settlementAreas = world->getSettlementAreas();
	std::vector<int> candidates(0);
	settlementAreas->intersectionFilter(homeBox,candidates);
	
	// Select Areas with maximum score.
	// Sort candidates following scores	
// uncomment it!!! 	
	std::make_heap(candidates.begin(),candidates.end(),compareSettlementAreas());		
	
	// Choosing one RANDOM richest area to pick a dune from it. TODO
	// richestCandidates.shuffle();
	int i = 0;	
	bool foundDune = false;
	while(!foundDune && i < candidates.size())
	{		
		Engine::Rectangle<int> selectedArea = settlementAreas->getAreaById(candidates[i]);
		Engine::Rectangle<int> intersection;
		// MRJ: If the selected area is outside of the "homeBox" then why caring about it all?
		//bool foo = homeBox.intersection(selectedArea, intersection);
		if ( !homeBox.intersection(selectedArea, intersection) ) continue;
		// Extract one random dune cell which is inside the homeRange and inside the selected area.
		
		// count dunes from candidate area "i", 'selectedArea' variable
		uint32_t countDunes = 0;
		Engine::Point2D<int> index;
		for (index._x = intersection._origin._x; index._x < intersection._origin._x+intersection._size._x; index._x++)			
		{
			for (index._y = intersection._origin._y; index._y < intersection._origin._y+intersection._size._y; index._y++)			
			{
				if ((world->getValue("soils",index) == DUNE) 
					&& (agent.getPosition().distance(index) <= (double)agent.getHomeMobilityRange()))
				{
					countDunes++;
				}
			}
		}		
		
		// pick one dune at random		
		uint32_t diceSelectOneRandomDune = world->getStatistics().getUniformDistValue(0, countDunes-1);
		for (index._x = intersection._origin._x; 
			 index._x < intersection._origin._x+intersection._size._x;
			 index._x++)			
		{
			for (index._y = intersection._origin._y; 
				 index._y < intersection._origin._y+intersection._size._y;
				 index._y++)				
			{
				if (world->getValue("soils",index) == DUNE
					&& (agent.getPosition().distance(index) <= (double)agent.getHomeMobilityRange())
					&& diceSelectOneRandomDune == 0.0)
				{		
					_newHomeLoc = index;
					foundDune   = true;
					break;
				}
				diceSelectOneRandomDune--;
			}
			if (foundDune)	
			{
				break;
			}
		}		
		if ( !foundDune )	// keep searching for a dune in next area
			{
			i++;
			}		
	}
	
	agent.getWorld()->setValue( "homeActivity", _newHomeLoc, prevHomeActivity + 1 );
	agent.setPosition( _newHomeLoc );
}

int MoveHomeAction::getTimeNeeded()
{
	return 5;//issue: set it in configuration file
}

} // namespace Gujarat

