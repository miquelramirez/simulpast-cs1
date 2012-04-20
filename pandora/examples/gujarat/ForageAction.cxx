#include "ForageAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"
#include "GujaratWorld.hxx"
#include "HunterGathererMDPState.hxx"
#include <cassert>

namespace Gujarat
{

ForageAction::ForageAction( Sector* loc, bool ownsPointer )
	: _forageArea( loc ), _ownsForageAreaPointer( ownsPointer )
{
}

ForageAction::~ForageAction()
{
	if ( _ownsForageAreaPointer )
		delete _forageArea;
}

Action*	ForageAction::copy() const
{
	if ( !_ownsForageAreaPointer )
		return new ForageAction( _forageArea, false );
	
	return new ForageAction( new Sector( *_forageArea ), true );
}

void	ForageAction::describe( std::ostream& os ) const
{
	os << "forage(" << _forageArea->biomassClass() << ")";
}


void	ForageAction::execute( GujaratAgent& a )
{
	std::cout << "[DEBUG]: Agent " << a.getId() << " is executing Forage action" << std::endl;

	HunterGatherer& agent = (HunterGatherer&) a;

	// 1. collect nr adults
	double  maxDistAgentWalk = agent.computeMaxForagingDistance();
		
	// 2. select nearest cell
	Engine::Point2D<int> nearest = _forageArea->getNearestTo( agent.getPosition() );

	// 3. execut walk
	int    amountCollected = doWalk( nearest, maxDistAgentWalk, agent );
	agent.updateResources( agent.convertBiomassToCalories(amountCollected) );
	std::cout << "[DEBUG][t=" << a.getWorld()->getCurrentTimeStep() << "]:";
	std::cout << "Agent " << a.getId() << " collects " << amountCollected << "grs of biomass, obtaining " << agent.convertBiomassToCalories(amountCollected) << "cal" << std::endl;
}

void	ForageAction::selectBestNearestCell( 	const Engine::Point2D<int>& n,
						const Engine::Raster& r,
						double& bestScore,
						Engine::Point2D<int>& best ) const
{
	// 1. Select adjacent cell max biomass*eff				
	std::vector< Engine::Point2D<int> >	adjCells;
	adjCells.clear();
	_forageArea->getAdjacent( n, adjCells );
	best = adjCells[0];
	bestScore = r.getValue( best );

	for ( unsigned i = 1; i < adjCells.size(); i++ ) 
	{
		double score = r.getValue( adjCells[i] );
		if ( score > bestScore )
		{
			bestScore = score;
			best = adjCells[i];
		}
	}
	
}

void	ForageAction::doWalk( const GujaratAgent& agent, const Engine::Point2D<int>& n0, double maxDist, Engine::Raster& r, int& collected ) const
{
	double walkedDist = 0.0;
	Engine::Point2D<int> n = n0;

	while ( walkedDist < maxDist )
	{
		Engine::Point2D<int> best;
		double bestScore;
		selectBestNearestCell( n, r, bestScore, best );
		// 2. update walk distance
		// MRJ: Walking from one place to another will consume very little time
		// we need to account for the time that hunting/gathering can take.
		// As a starting proposal, I'm setting the cost in time (i.e. travelling distance)
		// to be of 10 minutes (so the distance becomes  3 * 1 / 6 = 1/2 )
		walkedDist += 0.5;
		walkedDist += best.distance(n);
		n = best;
		
		int amtCollected = agent.computeEffectiveBiomassForaged( bestScore );
		collected += amtCollected;

		// 4. update cell resources & amount collected
		int prevValue = r.getValue(n); 
		r.setValue( n, prevValue - bestScore );
	}
}

int	ForageAction::doWalk( Engine::Point2D<int>& n0, double maxDist, GujaratAgent& agent )
{
	double walkedDist = 0.0;
	int    collected = 0;
	Engine::Point2D<int> n = n0;

	while ( walkedDist < maxDist )
	{
		Engine::Point2D<int> best;
		double bestScore;

		selectBestNearestCell( n, agent.getWorld()->getDynamicRaster("resources"), bestScore, best );
		// 2. update walk distance
		// MRJ: Walking from one place to another will consume very little time
		// we need to account for the time that hunting/gathering can take.
		// As a starting proposal, I'm setting the cost in time (i.e. travelling distance)
		// to be of 10 minutes (so the distance becomes  3 * 1 / 6 = 1/2 )
		walkedDist += 0.5;
		walkedDist += best.distance(n);
		n = best;

		// 3. collect resources depending on Normal( biomass*eff / 2 )
		int amtCollected = agent.computeEffectiveBiomassForaged( bestScore );
		int prevActivity = agent.getWorld()->getValue( "forageActivity", n );
		agent.getWorld()->setValue( "forageActivity", n, prevActivity + 1 );
		// MRJ: Mass to Cal conversion
		collected +=amtCollected ;

		// 4. update cell resources & amount collected 
		int prevValue = agent.getWorld()->getValue( "resources", n );
		agent.getWorld()->setValue( "resources", n, prevValue - bestScore );
	}
	
	return collected; 
}

void ForageAction::execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const
{
	//std::cout << "FORAGE" << std::endl;
	double  maxDist= agent.computeMaxForagingDistance();
		
	Engine::Point2D<int> nearest = _forageArea->getNearestTo( s.getLocation() );

	int collected = 0;

	doWalk( agent, nearest, maxDist, sp.getResourcesRaster(), collected );

	sp.addResources( agent.convertBiomassToCalories(collected));
}


}
