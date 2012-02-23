
#ifndef __Sector_hxx__
#define __Sector_hxx__

#include "Point2D.hxx"
#include <vector>
#include <iosfwd>

namespace Engine
{
	class World;
}

namespace Gujarat
{

enum	BiomassAmountClass
{
	BIOMASS_AMOUNT_LOW = 0,
	BIOMASS_AMOUNT_MED,
	BIOMASS_AMOUNT_HI
};

class Sector
{
	Engine::World & 			_world;
	std::vector< Engine::Point2D<int> >	_cells;
	int					_biomassAmount;
	BiomassAmountClass			_biomassAmountClass;

private:

	void	computeBiomassAmount();

public:
	Sector( Engine::World & world );
	virtual ~Sector();

	void	addCell( Engine::Point2D<int>& p )
	{
		_cells.push_back( p );
	}

	void	clearCells() 
	{
		_cells.clear();
	}

	int	getBiomassAmount() const
	{
		return _biomassAmount;
	}

	BiomassAmountClass	getBiomassAmountClass() const
	{
		return _biomassAmountClass;
	}

	void	updateFeatures();

	void	showFeatures( std::ostream& );
};

} // namespace Gujarat

#endif // __Sector_hxx__

