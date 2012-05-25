
#include <AlexisDemographics.hxx>

#include <Statistics.hxx>
#include <GeneralState.hxx>
#include <GujaratAgent.hxx>

namespace Gujarat
{

AlexisDemographics::AlexisDemographics( )	
{
}

AlexisDemographics::~AlexisDemographics()
{
}

bool	AlexisDemographics::checkEmigration( GujaratAgent& agent )
{
	float breakPoint = agent.getEmigrationProbability() * 10000.0f;	
	int die = Engine::GeneralState::statistics().getUniformDistValue(0,10000);
	agent.setEmigrationProbability( 0.0f );
	if ( die < breakPoint ) 
		return true;
	return false;
}


void AlexisDemographics::checkStarvationMortality( GujaratAgent& agent)
{
	//TODO should be called applyStarvationMortality()
	/*
	Man can starve himself for 70 days before dying, although some die
	after 17 days and the average is 40 days.
	
	Death results from general
	and deep catabolism of all body processes. The well nourished adult on a complete fast loses from 28 to 35 oz.
	weight daily. That is about 1% or 1 1/2% of total body weight. This loss
	continues almost constant for eight days. Then the average daily loss
	becomes smaller. Death occurs when from 35% to 40% of original weight
	has been sacrificed. 
	*/
	
	/*
	 mean survival days:40 days, interval:[17..70], (70-17)/2 = 87/2 = 43.5
	 
	 p = probab death, 1 day starvation
	 1-p = probab survival
	 
	 adults:
	 After 40 days of starvation, 50% of survivors
	 (1-p)^40 = .5, (1-p)? = SQRT_40(.5) = 0.999566876837
	 (1-p)^40 = .005, (1-p)? = SQRT_40(.005) = 0.996694028444
	 (1-p)^70 = .005, (1-p)? = SQRT_70(.005) = 0.998919295124
	 
	 children:
	 (1-p)^d=.01, d? let's say 20 days
	 
	 (1-p)^20=.01, (1-p)? = SQRT_20(0.01) = 0.988553094657
	 
	 +++++++++++++++++++++++-----------+++++++++
	 
	 ++++++++++++-++--+--++-+--++++++++++-+-+-++
	 
	 
	 */
	
	int needs = agent.computeConsumedResources(1);
	//log() << "CHECK/starvating> Agent_" << agent._id << "needs:" << needs << "resources:" << agent._collectedResources << std::endl;
	if (agent.getOnHandResources() < needs)
	{
		int deficitRatioPer1000 = (int)((1000.0f * (float)agent.getOnHandResources()) / (float)needs);
		for(unsigned int index=2; index < agent.getPopulationSize(); index++)
		{
			agent.checkIndividualStarvationDeath(index,deficitRatioPer1000);			
		}
		// ISSUE: Greater Ressilience for adults ??
		
		// Only child starvation, if adults should starve, then, emigrate!
		
	}	
}


void	AlexisDemographics::checkMortality( GujaratAgent& agent )
{
	// TODO:
	// Add attributes:
	// 	* Natural Adult Mortality (in 1000%)
	//	* Natural Children Mortality (in 1000%)
	//	* Elderly Age Threshold
	// Adult mortality check
	agent.checkDeath( 15, 1000, 2 );
	// Children mortality check
	agent.checkDeath( 0, 3, 10 );
	agent.checkDeath( 3, 15, 2 );
	// Old age death check
	agent.checkDeathByAging( 50 );
}

void	AlexisDemographics::checkReproduction( GujaratAgent& agent )
{
	if (!agent.canReproduce()) return;

	float breakPoint = agent.getReproductionProbability() * 10000.0f;
	int die = Engine::GeneralState::statistics().getUniformDistValue(0,10000);

	agent.setReproductionProbability(0.0f);

	if ( die < breakPoint )
		agent.addNewChild();
}

}
