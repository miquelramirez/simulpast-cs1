
#include "OriginalDemographics.hxx"

#include "Statistics.hxx"
#include "GeneralState.hxx"
#include "GujaratAgent.hxx"

namespace Gujarat
{

OriginalDemographics::OriginalDemographics()
{
}

OriginalDemographics::~OriginalDemographics()
{
}

bool	OriginalDemographics::checkEmigration(  GujaratAgent& agent )
{
	return false;
}

void	OriginalDemographics::checkMortality( GujaratAgent& agent )
{
	// TODO check resources, now year, must be seasonal
	int popSize = agent.getPopulationSize();
	// each individual eats 10 resources
	int maintainedPopulation = agent.getOnHandResources()/2000.0f;
	int starvingPopulation = popSize - maintainedPopulation;
	// for each starving pop, possibility of death = 10% for each individual
	if(starvingPopulation>0)
	{
		agent.decimatePopulation();
	}

	// Adult mortality check
	agent.checkDeath( 15, 1000, 2 );
	// Children mortality check
	agent.checkDeath( 0, 3, 10 );
	agent.checkDeath( 3, 15, 2 );
	// Old age death check
	agent.checkDeathByAging( 50 );
}

void	OriginalDemographics::checkReproduction( GujaratAgent& agent )
{
	if (!agent.canReproduce()) return;
	
	// 50% chance of having new child
	if(Engine::GeneralState::statistics().getUniformDistValue(0,1)==0)
		agent.addNewChild();
}

}
