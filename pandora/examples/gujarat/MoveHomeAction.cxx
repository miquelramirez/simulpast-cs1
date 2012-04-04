
#include "GujaratWorld.hxx"
#include "MoveHomeAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"
#include "HunterGathererMDPState.hxx"
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

void MoveHomeAction::generatePossibleActions( const GujaratAgent& agent,
						const Engine::Point2D<int>& agentPos,
						std::vector< MoveHomeAction* >& actions )
{
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
	const GujaratWorld * world              = (GujaratWorld *)agent.getWorld();
	const SettlementAreas * settlementAreas = world->getSettlementAreas();
	std::vector<int> candidates(0);
	settlementAreas->intersectionFilter(homeBox,candidates);
	
	// Select Areas with maximum score.
	// Sort candidates following scores	
	std::make_heap(candidates.begin(),candidates.end(),Gujarat::compareSettlementAreas(settlementAreas));		
	
	int i = 0;
	
	for ( unsigned i = 0; i < candidates.size(); i++ )
	{
		Engine::Rectangle<int> selectedArea = settlementAreas->getAreaById(candidates[i]);
		Engine::Rectangle<int> intersection;
		// MRJ: If the selected area is outside of the "homeBox" then why caring about it all?
		// ATM: above you will find settlementAreas->intersectionFilter method call. This ensures any
		// item inside candidates vector has intersection with homeBox. So the 'if' is not
		// needed, we just recompute the intersection to use it.
		bool foo = homeBox.intersection(selectedArea, intersection);
		//if ( !homeBox.intersection(selectedArea, intersection) ) continue;
		
		// Extract one random dune cell which is inside the homeRange and inside the selected area.
		
		// count dunes from candidate area "i", 'selectedArea' variable
		int countDunes = 0;
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
		
		bool foundDune = false;
		// pick one dune at random		
		uint32_t diceSelectOneRandomDune = world->getStatistics().getUniformDistValue(0, countDunes);
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
					actions.push_back( new MoveHomeAction( index ) );	
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
	}
	candidates.clear();
}

void MoveHomeAction::generatePossibleActions( 	GujaratAgent& agent,
						std::vector< MoveHomeAction* >& actions )
{
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
	std::make_heap(candidates.begin(),candidates.end(),Gujarat::compareSettlementAreas(settlementAreas));		
	
	int i = 0;
	
	for ( unsigned i = 0; i < candidates.size(); i++ )
	{
		Engine::Rectangle<int> selectedArea = settlementAreas->getAreaById(candidates[i]);
		Engine::Rectangle<int> intersection;
		// MRJ: If the selected area is outside of the "homeBox" then why caring about it all?
		// ATM: above you will find settlementAreas->intersectionFilter method call. This ensures any
		// item inside candidates vector has intersection with homeBox. So the 'if' is not
		// needed, we just recompute the intersection to use it.
		bool foo = homeBox.intersection(selectedArea, intersection);
		//if ( !homeBox.intersection(selectedArea, intersection) ) continue;
		
		// Extract one random dune cell which is inside the homeRange and inside the selected area.
		
		// count dunes from candidate area "i", 'selectedArea' variable
		int countDunes = 0;
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
		
		bool foundDune = false;
		// pick one dune at random		
		uint32_t diceSelectOneRandomDune = world->getStatistics().getUniformDistValue(0, countDunes);
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
					actions.push_back( new MoveHomeAction( index ) );	
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
	}
	candidates.clear();
}

void MoveHomeAction::execute( GujaratAgent & agent )
{
	std::cout << "DEBUG: MoveHome action executing..." << std::endl;
	int prevHomeActivity = agent.getWorld()->getValue( "homeActivity", _newHomeLoc );
	agent.getWorld()->setValue( "homeActivity", _newHomeLoc, prevHomeActivity + 1 );
	agent.setPosition( _newHomeLoc );
}

void MoveHomeAction::execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const
{
	sp.setLocation( _newHomeLoc );
	sp.decreaseResources( agent.computeConsumedResources(1) );
	sp.increaseTimeIndex();
}

int MoveHomeAction::getTimeNeeded() const
{
	return 5;//issue: set it in configuration file
}

} // namespace Gujarat

