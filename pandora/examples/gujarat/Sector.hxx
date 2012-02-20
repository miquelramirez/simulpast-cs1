
#ifndef __Sector_hxx__
#define __Sector_hxx__

#include "Point2D.hxx"
#include <vector>

namespace Engine
{
	class World;
}

namespace Gujarat
{


class Sector
{
	Engine::World & 			_world;
	std::vector< Engine::Point2D<int> >	_cells;

private:

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
};

} // namespace Gujarat

#endif // __Sector_hxx__

