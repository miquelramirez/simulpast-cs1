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

void	Sector::computeBiomassAmount()
{
	_biomassAmount = 0;
	int maxBiomassAmount = _world.getDynamicRaster("resources").getMaxValue();

	std::cout << "DEBUG: Max Timestep biomass amount" << maxBiomassAmount << std::endl;

	for ( unsigned i = 0; i < _cells.size(); i++ )
	{
		_biomassAmount += _world.getValue( "resources", _cells[i] );
	}
	
	double normAmount = (double)_biomassAmount / ((double)_cells.size()*maxBiomassAmount);

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

} // namespace Gujarat

