
#include "DoNothingAction.hxx"
#include "Agent.hxx"
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

MDPAction*	DoNothingAction::copy() const
{
	return new DoNothingAction();
}

void DoNothingAction::execute( Engine::Agent& agent )
{
	std::cout << "[DEBUG]: Agent " << agent.getId() << " is executing DoNothing action" << std::endl; 
}

void DoNothingAction::generatePossibleActions( GujaratAgent& agent, std::vector< DoNothingAction* >& actions )
{
	actions.push_back( new DoNothingAction() );
}

void DoNothingAction::executeMDP( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const
{
	//std::cout << "DO NOTHING" << std::endl;
}

std::string DoNothingAction::describe() const
{
	return "do_nothing()";
}


}
