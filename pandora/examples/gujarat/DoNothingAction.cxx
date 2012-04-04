#include "DoNothingAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGathererMDPState.hxx"

namespace Gujarat
{

DoNothingAction::DoNothingAction()
{
}

DoNothingAction::~DoNothingAction()
{
}

void	DoNothingAction::execute( GujaratAgent& agent )
{
}

void	DoNothingAction::generatePossibleActions( GujaratAgent& agent, std::vector< DoNothingAction* >& actions )
{
	actions.push_back( new DoNothingAction() );
}

void	DoNothingAction::execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const
{
	sp.decreaseResources( agent.computeConsumedResources(1) );
	sp.increaseTimeIndex();	
}


}
