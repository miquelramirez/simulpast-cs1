#include <iostream>
#include "Sector.hxx"
#include "World.hxx"

namespace Gujarat
{

Sector::Sector( Engine::World & world ) 
	: _world(world), _biomassAmount(0)
{
}

Sector::~Sector()
{
}

Engine::Point2D<int>	Sector::getNearestTo( Engine::Point2D<int> p )
{
	Engine::Point2D<int> nearest = _cells[0];
	double nearestDist = nearest.distance( p );

	for ( unsigned i = 1; i < _cells.size(); i++ )
	{
		double currDist = _cells[i].distance( p );
		if ( currDist < nearestDist )
		{
			nearest = _cells[i];
			nearestDist = currDist;
		} 
	} 

	return nearest;
}

void	Sector::computeBiomassAmount()
{
	_biomassAmount = 0;
	int maxBiomassAmount = _world.getDynamicRaster("resources").getCurrentMaxValue();

	std::cout << "DEBUG: Max Timestep biomass amount" << maxBiomassAmount << std::endl;

	for ( unsigned i = 0; i < _cells.size(); i++ )
	{
		_biomassAmount += _world.getValue( "resources", _cells[i] );
	}
	
	double normAmount = (double)_biomassAmount;
	if ( maxBiomassAmount > 0 )
		normAmount /= ((double)_cells.size()*maxBiomassAmount);
	else
		normAmount = 0.0;

	std::cout << "DEBUG: biomass amount: " << _biomassAmount;
	std::cout << " # cells: " << _cells.size() << " max bio: " << maxBiomassAmount;
	std::cout << " (" << _cells.size()*maxBiomassAmount << ")" << std::endl;
	std::cout << "DEBUG: normalized biomass amount" << normAmount << std::endl;
	
	if ( normAmount <= 1.0/3.0)
		_biomassAmountClass = BIOMASS_AMOUNT_LOW;
	else if ( normAmount <= 2.0/3.0 )
		_biomassAmountClass = BIOMASS_AMOUNT_MED;
	else
		_biomassAmountClass = BIOMASS_AMOUNT_HI;
	
}

void	Sector::updateFeatures()
{
	computeBiomassAmount();
}

void	Sector::showFeatures( std::ostream& stream )
{
	stream << "\tFeature: BioMassAmount: " << _biomassAmount << std::endl;
	
	std::string bioclass;
	switch( (unsigned)_biomassAmountClass ) 
	{
	case BIOMASS_AMOUNT_LOW :
		bioclass = "LOW";
		break;
	case BIOMASS_AMOUNT_MED :
		bioclass = "MED";
		break;
	case BIOMASS_AMOUNT_HI :
		bioclass = "HI";
		break;
	}
	stream << "\tFeature: BioMassAmountClass: " << bioclass << std::endl;
}

void	Sector::getAdjacent( Engine::Point2D<int> p, std::vector<Engine::Point2D<int> >& adjList )
{
	for ( unsigned i = 0; i < _cells.size(); i++ )
	{
		Engine::Point2D<int> delta = p - _cells[i];
		delta._x = abs(delta._x);
		delta._y = abs(delta._y);	
		if ( delta._x <= 1 && delta._y <= 1 )
			adjList.push_back( _cells[i] );
	}	
}

} // namespace Gujarat

