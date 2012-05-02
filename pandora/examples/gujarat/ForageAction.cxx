#include "ForageAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"
#include "GujaratWorld.hxx"
#include "HunterGathererMDPState.hxx"
#include <cassert>

namespace Gujarat
{

ForageAction::ForageAction( Sector* loc, bool ownsPointer )
	: _forageArea( loc ), _ownsForageAreaPointer( ownsPointer ), _biomassCollected(0), _caloriesCollected(0)
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
	os << "forage(" << _forageArea->biomassClass() << ")," << getBiomassCollected() << "," << getCaloriesCollected();
}


void	ForageAction::execute( GujaratAgent& a )
{
	HunterGatherer& agent = (HunterGatherer&) a;

	// 1. collect nr adults
	double  maxDistAgentWalk = agent.computeMaxForagingDistance();
		
	// 2. select nearest cell
	Engine::Point2D<int> nearest = _forageArea->getNearestTo( agent.getPosition() );

	// 3. execute walk
	_biomassCollected = doWalk( nearest, maxDistAgentWalk, agent );
	_caloriesCollected = agent.convertBiomassToCalories( _biomassCollected );
	agent.updateResources( _caloriesCollected );
}

void	ForageAction::selectBestNearestCell( 	const Engine::Point2D<int>& n,
						const Engine::Raster& r,
						int& bestScore,
						Engine::Point2D<int>& best ) const
{
	bestScore = 0;
	double minDist = 1e20;

	const std::vector< Engine::Point2D<int> >& sectorCells = _forageArea->cells();

	for ( unsigned k = 0; k < _forageArea->numCells(); k++ )
	{
		int score =  r.getValue( sectorCells[k] );
		double dist = sectorCells[k].distance(n);
		if ( score > bestScore )
		{
			bestScore = score;
			best = sectorCells[k];
			minDist = dist; 
		}
		else if ( score == bestScore && minDist > dist )
		{
			best = sectorCells[k];
			minDist = dist;
		}
	}
}

void	ForageAction::doWalk( const GujaratAgent& agent, const Engine::Point2D<int>& n0, double maxDist, Engine::Raster& r, int& collected ) const
{
	double walkedDist = 0.0;
	Engine::Point2D<int> n = n0;
	double distHome = 0.0;

	while ( ( walkedDist + distHome ) < maxDist )
	{
		Engine::Point2D<int> best;
		int bestScore = 0;
		selectBestNearestCell( n, r, bestScore, best );
		// 2. update walk distance
		walkedDist += agent.getTimeSpentForagingTile();
		walkedDist += best.distance(n);
		n = best;
		distHome = n0.distance(n);	
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
	double	distHome = 0.0;

	while ( ( walkedDist + distHome ) < maxDist )
	{
		Engine::Point2D<int> best;
		int bestScore = 0;

		selectBestNearestCell( n, agent.getWorld()->getDynamicRaster("resources"), bestScore, best );
		// 2. update walk distance
		walkedDist += agent.getTimeSpentForagingTile();
		walkedDist += best.distance(n);
		n = best;
		distHome = n0.distance(n);

		// 3. collect resources depending on Normal( biomass*eff / 2 )
		int amtCollected = agent.computeEffectiveBiomassForaged( bestScore );
		int prevActivity = agent.getWorld()->getValue( "forageActivity", n );
		agent.getWorld()->setValue( "forageActivity", n, prevActivity + 1 );
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
