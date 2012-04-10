#include <Action.hxx>
#include <Exceptions.hxx>
#include "HunterGathererMDPState.hxx"

namespace Gujarat
{

Action::Action()
{
}

Action::~Action()
{
}

void	Action::execute( const GujaratAgent& agent, const HunterGathererMDPState& s, 
			HunterGathererMDPState& sp ) const
{
	throw Engine::Exception( "Not Implemented for HunterGatherer MDP states!" );
}

} // namespace Gujarat
