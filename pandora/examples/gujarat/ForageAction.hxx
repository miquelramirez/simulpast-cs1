#ifndef __ForageAction_hxx__
#define __ForageAction_hxx__

#include "Action.hxx"
#include "Sector.hxx"
#include "Point2D.hxx"

namespace Gujarat
{

class GujaratAgent;
class GujaratWorld;

class ForageAction : public Action
{
private:

	Sector*		_forageArea;
	bool		_ownsForageAreaPointer;
	int		_biomassCollected;
	int		_caloriesCollected;

	void		selectBestNearestCell( 	const Engine::Point2D<int>& current,
						const Engine::Raster& r,
						int& bestScore,
						Engine::Point2D<int>& best ) const;

	void		doWalk( const GujaratAgent& agent, 
				const Engine::Point2D<int>& n0, 
				double maxDist, 
				Engine::Raster& r, 
				int& collected ) const;

	void		doWalk( GujaratAgent& agent, 
				const Engine::Point2D<int>& n0, 
				double maxDist, 
				Engine::Raster& r, 
				int& collected );

public:
	ForageAction( Sector* loc, bool ownsPointer = false );
	virtual ~ForageAction();

	void	execute( GujaratAgent& agent );
	int	getTimeNeeded() const { return 1; }
	virtual void execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const;
	virtual Action* copy() const;
	virtual void	describe( std::ostream& os ) const;
	
	int	getBiomassCollected() const { return _biomassCollected; }
	int	getCaloriesCollected() const { return _caloriesCollected; }
};

}

#endif // ForageAction.hxx
