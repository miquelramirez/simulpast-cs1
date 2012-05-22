#include "OriginalDemographics.hxx"

namespace Gujarat
{

OriginalDemographics::OriginalDemographics( GujaratAgent& agent )
	: GujaratDemographics( agent )
{
}

OriginalDemographics::~OriginalDemographics()
{
}

bool	OriginalDemographics::checkEmigration( )
{
	return false;
}

void	OriginalDemographics::checkMortality()
{
	// TODO check resources, now year, must be seasonal
	int popSize = _agent.getPopulationSize();
	// each individual eats 10 resources
	int maintainedPopulation = _agent.getOnHandResources()/2000.0f;
	int starvingPopulation = popSize - maintainedPopulation;
	// for each starving pop, possibility of death = 10% for each individual
	if(starvingPopulation>0)
	{
		_agent.decimatePopulation();
	}

	// Adult mortality check
	_agent.checkDeath( 15, 1000, 2 );
	// Children mortality check
	_agent.checkDeath( 0, 3, 10 );
	_agent.checkDeath( 3, 15, 2 );
	// Old age death check
	_agent.checkDeathByAging( 50 );
}

void	OriginalDemographics::checkReproduction()
{
	if (!_agent.canReproduce()) return;
	
	// 50% chance of having new child
	if(_agent.getWorld()->getStatistics().getUniformDistValue(0,1)==0)
		_agent.addNewChild();
}

}
