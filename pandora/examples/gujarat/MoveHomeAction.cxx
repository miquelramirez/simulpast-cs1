
#include "GujaratWorld.hxx"
#include "MoveHomeAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

MoveHomeAction::MoveHomeAction()
{
}

MoveHomeAction::~MoveHomeAction()
{
}

void MoveHomeAction::execute( GujaratAgent & agent )
{
	std::vector<Engine::Point2D<int> > possiblePositions;
	// TODO this method excludes the creation of children in cells in other environments
	Engine::Point2D<int> location;
	Engine::Point2D<int> currPos = agent.getPosition();
	int range = agent.getHomeMobilityRange();

	std::cout << "DEBUG: MoveHome action executing..." << std::endl;
	
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
				possiblePositions.push_back(location);
			}
		}
	}

	// no one near the agent
	if(possiblePositions.size()==0)
	{
		return;
	}
	std::random_shuffle(possiblePositions.begin(), possiblePositions.end());
	agent.setPosition( possiblePositions[0] );
}

int MoveHomeAction::getTimeNeeded()
{
	return 5;
}

} // namespace Gujarat

