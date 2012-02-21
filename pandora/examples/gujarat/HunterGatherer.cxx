
#include "HunterGatherer.hxx"
#include "GujaratWorld.hxx"
#include "Exceptions.hxx"
//#include "Action.hxx"
#include "MoveHomeAction.hxx"
#include "GatherAction.hxx"
#include "HuntAction.hxx"
#include "Sector.hxx"
#include <cmath>

namespace Gujarat
{

HunterGatherer::HunterGatherer( const std::string & id ) 
	: GujaratAgent(id), _surplusForReproductionThreshold(2), _surplusWanted(1), _homeRange(50),
	_numSectors( 4 )
{
}

HunterGatherer::~HunterGatherer()
{
	for ( unsigned k = 0; k < _numSectors; k++ )
		delete _sectors[k];
}

bool HunterGatherer::sameSide( Engine::Point2D<int> A, Engine::Point2D<int> B, Engine::Point2D<int> P )
{
	Engine::Point2D<int> BA = B - A;
	Engine::Point2D<int> PA = P - A;

	return BA.dot(PA) >= 0;	
}

bool HunterGatherer::insideTriangle( Engine::Point2D<int> p,  Engine::Point2D<int> b, Engine::Point2D<int> c )
{
	Engine::Point2D<int> a(0,0);

	if ( sameSide( a, b, p ) && sameSide( b, c, p) && sameSide( c, a, p ) )
		return true;
	if ( !sameSide( a, b, p ) && !sameSide( b, c, p) && !sameSide( c, a, p ) )
		return true;
	return false;

}

void HunterGatherer::createSectorsMask()
{
	std::vector< std::vector< Engine::Point2D<int> > > sectors;
	
	float alpha = 360/_numSectors;
	alpha = alpha * M_PI/180.0f;
	Engine::Point2D<float> b;
	Engine::Point2D<float> c;

	sectors.resize( _numSectors );
	_sectorsMask.resize( 2*_homeRange );
	for ( unsigned k = 0; k < 2*_homeRange; k++ )
		_sectorsMask[k].resize( 2*_homeRange );

	b._x = 0;
	b._y = - _homeRange;

	for(int i=0; i<_numSectors; i++)
	{
		c._x = b._x*std::cos(alpha) - b._y*std::sin(alpha);
		c._y = b._x*std::sin(alpha) + b._y*std::cos(alpha);
		sectors[i].push_back( Engine::Point2D<int>( (int)b._x, (int)b._y ) );
		sectors[i].push_back( Engine::Point2D<int>( (int)c._x, (int)c._y ) );
		b = c;
	}


	int x, y;

	for ( x = -_homeRange; x < _homeRange; x++ )
		for ( y = -_homeRange; y < _homeRange; y++ )
		{
			Engine::Point2D<int> p( x, y );
			_sectorsMask[x+_homeRange][y+_homeRange] = -1;	
			for ( unsigned k = 0; k < _numSectors; k++ )
			{
				if ( insideTriangle( p, sectors[k][0], sectors[k][1] ) )
				{
					_sectorsMask[x+_homeRange][y+_homeRange] = k;
					break;
				}
			}

		}

	_sectors.resize( _numSectors );
	for ( unsigned k = 0; k < _numSectors; k++ )
		_sectors[k] = new Sector( *_world );
}

void HunterGatherer::updateKnowledge()
{
	for ( unsigned k = 0; k < _numSectors; k++ )
		_sectors[k]->clearCells();

	int x, y;

	for ( x = -_homeRange; x < _homeRange; x++ )
		for ( y = -_homeRange; y < _homeRange; y++ )
		{
			int indexSector = _sectorsMask[x+_homeRange][y+_homeRange];
			if ( indexSector == - 1 ) continue;
			Engine::Point2D<int> p;
			p._x = _position._x + x;
			p._y = _position._y + y;
			if ( !_world->getBoundaries().isInside(p) )
				continue;
			_sectors[indexSector]->addCell( p );
		}	
	
}

void HunterGatherer::evaluateYearlyActions()
{
}

void HunterGatherer::evaluateSeasonalActions()
{
}

void HunterGatherer::evaluateIntraSeasonalActions()
{
	// TODO: which order must follow the actions? random?
	// now random

	// action pack : move Home, hunting, gathering
	int dice = _world->getStatistics().getUniformDistValue(1,10);

	if ( dice >= 8 ) // p=0.2 agent chooses to move its home
	{
		_actions.push_back( new MoveHomeAction() );
		return;
	}

	if ( dice < 4 ) // select Hunt
	{
		_actions.push_back( new HuntAction() );
		return;
	}

	_actions.push_back( new GatherAction() );

}

void * HunterGatherer::createPackage()
{
	return 0;
}

void HunterGatherer::serializeAdditionalAttributes()
{
}

GujaratAgent * HunterGatherer::createNewAgent()
{	
	GujaratWorld * world = (GujaratWorld*)_world;
	std::ostringstream oss;
	oss << "HunterGatherer_" << world->getId() << "-" << world->getNewKey();
	return new HunterGatherer(oss.str());
}

bool	HunterGatherer::needsResources()
{
	return _collectedResources < (_surplusForReproductionThreshold + _surplusWanted);
}

bool	HunterGatherer::cellValid( Engine::Point2D<int>& loc )
{
	if ( !_world->getOverlapBoundaries().isInside(loc) )
		return false;
	// Check that the home of another agent resides in loc
	Agent * agent = _world->getAgent(loc);
	if ( agent && agent->exists() && (agent != this ) )
		return false;
	return true;
}

bool	HunterGatherer::cellRelevant( Engine::Point2D<int>& loc )
{
	Soils soilType = (Soils) _world->getValue("soils", loc);
	int resourceType = _world->getValue("resourceType", loc);
	return soilType == INTERDUNE && resourceType == WILD;
}

} // namespace Gujarat

