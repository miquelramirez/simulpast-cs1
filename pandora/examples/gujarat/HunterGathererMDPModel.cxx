#include "HunterGathererMDPModel.hxx"
#include "HunterGatherer.hxx"
#include "HunterGathererMDPConfig.hxx"
#include "ForageAction.hxx"
#include "MoveHomeAction.hxx"
#include "DoNothingAction.hxx"
#include "Exceptions.hxx"
#include <typeinfo>

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
	std::cout << "Initial state: " << *_initial << std::endl;	
}

action_t	HunterGathererMDPModel::number_actions( const HunterGathererMDPState& s ) const
{
	return s.numAvailableActions();
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
	s.initializeSuccessor(sp);
	const Action* act = s.availableActions(a);
	act->execute( agentRef(), s, sp );
	applyFrameEffects( s, sp );
	makeActionsForState( sp );
	outcomes.push_back( std::make_pair(sp, 1.0) );
}

void	HunterGathererMDPModel::applyFrameEffects( const HunterGathererMDPState& s,  HunterGathererMDPState& sp ) const
{
	sp.decreaseResources( agent.computeConsumedResources(1) );
	sp.increaseTimeIndex();	
}

void	HunterGathererMDPModel::makeActionsForState( HunterGathererMDPState& s ) const
{
	assert( s.numAvailableActions() == 0 );
	// Make Do Nothing
	s.addAction( new DoNothingAction() );	
	
	// Make Forage actions
	std::vector< Sector* > actionSectors;

	agentRef().updateKnowledge( s.getLocation(), s.getResourcesRaster(), actionSectors );

	if ( _config.getNumberForageActions() > actionSectors.size() )
	{
		throw Engine::Exception( "HunterGathererMDPModel::makeActionsForState() : nr. forage actions in model > nr. available sectors" );
	}	
	else if ( _config.getNumberForageActions() == actionSectors.size() )
	{
		for ( unsigned i = 0; i < actionSectors.size(); i++ )
			s.addAction( new ForageAction( actionSectors[i], true ) );	
	}
	else
	{
		std::sort( actionSectors.begin(), actionSectors.end(), SectorBestFirstSortPtrVecPredicate() );
		for ( unsigned i = 0; i < _config.getNumberForageActions(); i++ )
			s.addAction( new ForageAction( actionSectors[i], true ) );
		for ( unsigned i = _config.getNumberForageActions(); i < actionSectors.size(); i++ )
			delete actionSectors[i];
	}
	
	// Make Move Home
	std::vector< MoveHomeAction* > possibleMoveHomeActions;
	MoveHomeAction::generatePossibleActions( agentRef(), s.getLocation(), possibleMoveHomeActions );

	if ( _config.getNumberMoveHomeActions() >=  possibleMoveHomeActions.size() )
	{
		for ( unsigned i = 0; i < possibleMoveHomeActions.size(); i++ )
			s.addAction( possibleMoveHomeActions[i] );
	}
	else
	{
		for ( unsigned i = 0; i < _config.getNumberMoveHomeActions(); i++ )
			s.addAction( possibleMoveHomeActions[i] );
		for ( unsigned i = _config.getNumberMoveHomeActions(); i < possibleMoveHomeActions.size(); i++ )
			delete possibleMoveHomeActions[i];
	}
	assert( s.numAvailableActions() > 0 );
} 

}
