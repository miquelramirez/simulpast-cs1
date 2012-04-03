#include "HunterGathererMDPModel.hxx"
#include "HunterGatherer.hxx"
#include "HunterGathererMDPConfig.hxx"
#include "ForageAction.hxx"
#include "MoveHomeAction.hxx"
#include "DoNothingAction.hxx"

using Problem::action_t;

namespace Gujarat
{

HunterGathererMDPModel::HunterGathererMDPModel( HunterGatherer* a )
	: _simAgent( a ), _initial( NULL )
{
}

HunterGathererMDPModel::~HunterGathererMDPModel()
{
	if ( _initial != NULL )
		delete _initial;
}

void	HunterGathererMDPModel::setup( const HunterGathererMDPConfig& cfg )
{
	_config = cfg;
	setHorizon( cfg.getHorizon() );
}

void	HunterGathererMDPModel::reset()
{
	if ( _initial != NULL )
		delete _initial;

	// Build initial state from current state in the simulation
	_initial = new HunterGathererMDPState(	agentRef().getPosition(),
						agentRef().getOnHandResources(),
						agentRef().getWorld()->getDynamicRaster( "resources" ) );
	makeActionsForState( *_initial );
							
}

action_t	HunterGathererMDPModel::number_actions( const HunterGathererMDPState& s ) const
{
	return _config.getNumberForageActions() + _config.getNumberMoveHomeActions() +
		( _config.isDoNothingAllowed() ? 1 : 0 );
}

const HunterGathererMDPState& HunterGathererMDPModel::init() const
{
	return *_initial;
}

bool HunterGathererMDPModel::terminal( const HunterGathererMDPState& s ) const
{
	return s.getTimeIndex() == getHorizon();
}

bool HunterGathererMDPModel::applicable( const HunterGathererMDPState& s,
						action_t a ) const
{
	return true;
}

float HunterGathererMDPModel::cost( const HunterGathererMDPState& s,
					action_t a ) const
{
	float rewardSignal = 1000.0f - (float)s.getOnHandResources();
	rewardSignal += s.availableActions(a)->getTimeNeeded();
	return rewardSignal;
}

void HunterGathererMDPModel::next( 	const HunterGathererMDPState &s, 
					action_t a, 
					OutcomeVector& outcomes ) const
{
	HunterGathererMDPState sp;
	s.availableActions(a)->execute( agentRef(), s, sp );
	makeActionsForState( sp );
	outcomes.push_back( std::make_pair(sp, 1.0) );
}

void	HunterGathererMDPModel::makeActionsForState( HunterGathererMDPState& s ) const
{
} 

}
