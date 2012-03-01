#include "ForageAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

ForageAction::ForageAction( Sector* loc )
	: _forageArea( loc )
{
}

ForageAction::~ForageAction()
{
}

void	ForageAction::execute( GujaratAgent& a )
{
	HunterGatherer& agent = (HunterGatherer&) a;

	// 1. collect nr adults
	int 	nAdults = agent.getNrAvailableAdults();		
	double  walkingSpeedHour = 3.0;
	double  availTime = 4.5;
	double  distPerAdult = walkingSpeedHour * availTime;
	double  maxDistAgentWalk = distPerAdult * (double)nAdults;
	
	// 2. select nearest cell
	Engine::Point2D<int> nearest = _forageArea->getNearestTo( agent.getPosition() );

	// 3. execut walk
	int    amountCollected = doWalk( nearest, maxDistAgentWalk, agent.getWorld() );
	agent.updateResources( amountCollected );
}


int	ForageAction::doWalk( Engine::Point2D<int>& n0, double maxDist, Engine::World* world )
{
	double walkedDist = 0.0;
	int    collected = 0;
	std::vector< Engine::Point2D<int> >	adjCells;
	Engine::Point2D<int> n = n0;

	while ( walkedDist < maxDist )
	{
		// 1. Select adjacent cell max biomass*eff				
		adjCells.clear();
		_forageArea->getAdjacent( n, adjCells );
		Engine::Point2D<int> best = adjCells[0];
		double bestScore = world->getValue( "resources", best );

		for ( unsigned i = 1; i < adjCells.size(); i++ ) 
		{
			double score = world->getValue( "resources", adjCells[i] );
			if ( score > bestScore )
			{
				bestScore = score;
				best = adjCells[i];
			}
		}
		// 2. update walk distance
		walkedDist += best.distance(n);
		n = best;

		// 3. collect resources depending on Normal( biomass*eff / 2 )
		int amtCollected =  world->getStatistics().getNormalDistValue(0,bestScore);
		int prevActivity = world->getValue( "forageActivity", n );
		world->setValue( "forageActivity", n, prevActivity + 1 );
		collected += amtCollected;

		// 4. update cell resources & amount collected 
		world->setValue( "resources", n, bestScore - amtCollected );
	}
	
	return collected; 
}

}
