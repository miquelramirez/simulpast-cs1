#include "HunterGathererProgrammedController.hxx"
#include <vector>
#include <iostream>
#include "MoveHomeAction.hxx"
#include "ForageAction.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

HunterGathererProgrammedController::HunterGathererProgrammedController( GujaratAgent* a )
	: AgentController( a )
{
	_agentConcrete = dynamic_cast<HunterGatherer*>( a );
}

HunterGathererProgrammedController::~HunterGathererProgrammedController()
{
}

Action*	HunterGathererProgrammedController::selectAction()
{
	// TODO: which order must follow the actions? random?
	// now random

	// action pack : move Home, hunting, gathering
	int dice = agentRef().getWorld()->getStatistics().getUniformDistValue(1,10);

	if ( dice >= 8 ) // p=0.2 agent chooses to move its home
	{
		std::cout << "DEBUG: MoveHome action selected" << std::endl;
		std::vector< MoveHomeAction* > possibleActions;
		MoveHomeAction::generatePossibleActions( agentRef(), possibleActions );

		// MRJ: Select Move Home action on a random basis
		dice = agentRef().getWorld()->getStatistics().getUniformDistValue( 0, possibleActions.size() - 1 );

		MoveHomeAction* selectedAction = possibleActions[dice];
		possibleActions[dice] = NULL;
		for ( unsigned i = 0; i < possibleActions.size(); i++ )
			if ( possibleActions[i] != NULL )
				delete possibleActions[i];
		return selectedAction;
	}

	do
	{
		dice = _agentConcrete->getWorld()->getStatistics().getUniformDistValue( 0, _agentConcrete->getSectors().size()-1 );
	
	} while ( _agentConcrete->getSectors()[dice]->isEmpty() );

	return new ForageAction( _agentConcrete->getSectors()[dice] );
	
}

}
