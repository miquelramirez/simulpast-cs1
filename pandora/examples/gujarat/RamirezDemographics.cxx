#include "RamirezDemographics.hxx"

namespace Gujarat
{

RamirezDemographics::RamirezDemographics( GujaratAgent& agent )
	: GujaratDemographics( agent )
{
}

RamirezDemographics::~RamirezDemographics()
{
}

bool	RamirezDemographics::checkEmigration( )
{
	float breakPoint = _agent.getEmigrationProbability() * 10000.0f;
	int die = _agent.getWorld()->getStatistics().getUniformDistValue(0,10000);
	_agent.setEmigrationProbability( 0.0f );
	if ( die < breakPoint ) 
		return true;
	return false;
}

void	RamirezDemographics::checkMortality()
{
	// Adult mortality check
	_agent.checkDeath( 15, 1000, 2 );
	// Children mortality check
	_agent.checkDeath( 0, 3, 10 );
	_agent.checkDeath( 3, 15, 2 );
	// Old age death check
	_agent.checkDeathByAging( 50 );
}

void	RamirezDemographics::checkReproduction()
{
	if (!_agent.canReproduce()) return;

	float breakPoint = _agent.getReproductionProbability() * 10000.0f;
	int die = _agent.getWorld()->getStatistics().getUniformDistValue(0,10000);

	_agent.setReproductionProbability(0.0f);

	if ( die < breakPoint )
		_agent.addNewChild();
}

}
