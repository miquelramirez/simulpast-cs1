#include "DoNothingAction.hxx"

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

}
