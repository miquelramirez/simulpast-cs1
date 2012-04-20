#include <Action.hxx>
#include <Exceptions.hxx>
#include <iostream>
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

void	Action::describe( std::ostream& os ) const
{
}

} // namespace Gujarat
