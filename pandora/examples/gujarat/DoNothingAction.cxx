#include "DoNothingAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGathererMDPState.hxx"
#include <iostream>

namespace Gujarat
{

DoNothingAction::DoNothingAction()
{
}

DoNothingAction::~DoNothingAction()
{
}

Action*	DoNothingAction::copy() const
{
	return new DoNothingAction();
}

void	DoNothingAction::execute( GujaratAgent& agent )
{
	std::cout << "[DEBUG]: Agent " << agent.getId() << " is executing DoNothing action" << std::endl; 
}

void	DoNothingAction::generatePossibleActions( GujaratAgent& agent, std::vector< DoNothingAction* >& actions )
{
	actions.push_back( new DoNothingAction() );
}

void	DoNothingAction::execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const
{
	//std::cout << "DO NOTHING" << std::endl;
}


}
