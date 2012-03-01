
#include "GujaratWorld.hxx"
#include "MoveHomeAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

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
	agent.getWorld()->setValue( "homeActivity", _newHomeLoc, prevHomeActivity + 1 );
	agent.setPosition( _newHomeLoc );
}

int MoveHomeAction::getTimeNeeded()
{
	return 5;
}

} // namespace Gujarat

