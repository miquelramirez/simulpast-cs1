
#include <HunterGatherer.hxx>
#include <GujaratWorld.hxx>
#include <Exceptions.hxx>
#include <Action.hxx>
#include <Sector.hxx>
#include <cmath>
#include <cassert>
#include <AgentController.hxx>

#include <GujaratState.hxx>

namespace Gujarat
{

HunterGatherer::HunterGatherer( const std::string & id ) 
	: GujaratAgent(id), _surplusForReproductionThreshold(2), _surplusWanted(1), _homeRange(50),
	_numSectors( -1 )
{
}

HunterGatherer::~HunterGatherer()
{
	for ( unsigned k = 0; k < _sectors.size(); k++ )
	{
		delete _sectors[k];
	}
}

void HunterGatherer::updateKnowledge( 	const Engine::Point2D<int>& agentPos,
					const Engine::Raster& dataRaster, 
					std::vector<Sector*>& sectors ) const
{
	for ( unsigned k = 0; k < _numSectors; k++ )
	{
		sectors.push_back( new Sector() );
	}

	for ( int x=-_homeRange; x<=_homeRange; x++ )
	{
		for ( int y=-_homeRange; y<=_homeRange; y++ )
		{
			int indexSector = GujaratState::sectorsMask(x+_homeRange,y+_homeRange);
			if ( indexSector == - 1 )
			{
				continue;
			}

			Engine::Point2D<int> p;
			p._x = agentPos._x + x;
			p._y = agentPos._y + y;
			if ( !_world->getBoundaries().isInside(p) )
			{
				continue;
			}
			sectors[indexSector]->addCell( p );
		}
	}

	for ( unsigned k = 0; k < _numSectors; k++ )
	{
		sectors[k]->updateFeatures(dataRaster);
	}

}

void HunterGatherer::updateKnowledge()
{
	// sectors not initialized
	if(_sectors.size()==0)
	{
		_sectors.resize(_numSectors);
		for ( unsigned k = 0; k < _numSectors; k++ )
		{
			_sectors[k] = new Sector( _world );
		}
	}
	// if initialized, clean past computations
	else
	{
		for ( unsigned k = 0; k < _numSectors; k++ )
		{
			//std::cout << this << "clearing sector: " << k << std::endl;
			_sectors[k]->clearCells();
			//std::cout << "DONE!" <<  std::endl;
		}
	}

	for ( int x=-_homeRange; x<=_homeRange; x++ )
	{
		for ( int y=-_homeRange; y<=_homeRange; y++ )
		{
			int indexSector = GujaratState::sectorsMask(x+_homeRange,y+_homeRange);
			if ( indexSector == - 1 )
			{
				continue;
			}

			Engine::Point2D<int> p;
			p._x = _position._x + x;
			p._y = _position._y + y;
			if ( !_world->getBoundaries().isInside(p) )
			{
				continue;
			}
			_sectors[indexSector]->addCell( p );
			getWorld()->setValue( "sectors", p, 1 );
		}
	}

	for ( unsigned k = 0; k < _numSectors; k++ )
	{
		//std::cout << "Sector #" << (k+1) << " features:" << std::endl;
		_sectors[k]->updateFeatures();
		//_sectors[k]->showFeatures( std::cout );
	}
}

void HunterGatherer::selectActions()
{
	_actions.push_back( (Engine::Action*)(GujaratState::controller().selectAction(*this)) );
}

void HunterGatherer::serializeAdditionalAttributes()
{
	serializeAttribute("collected resources", _collectedResources);
}

GujaratAgent * HunterGatherer::createNewAgent()
{	
	std::cout << "creating new agent" << std::endl;
	GujaratWorld * world = (GujaratWorld*)_world;
	std::ostringstream oss;
	oss << "HunterGatherer_" << world->getId() << "-" << world->getNewKey();
	
	HunterGatherer * agent = new HunterGatherer(oss.str());
	_world->addAgent(agent);
	
	agent->setAvailableTime( _availableTime );
	agent->setSocialRange( _socialRange );
	agent->setHomeMobilityRange( _homeMobilityRange );
	agent->setHomeRange( _homeRange );
	agent->setSurplusForReproductionThreshold( _surplusForReproductionThreshold );
	agent->setSurplusWanted( _surplusWanted );
	agent->setNumSectors( _sectors.size() );
	agent->setPosition(_position);

	return agent;
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

