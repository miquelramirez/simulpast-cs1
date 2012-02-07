
#include "MoveHomeAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

MoveHomeAction::MoveHomeAction()
{
}

MoveHomeAction::~MoveHomeAction()
{
}

void MoveHomeAction::execute( GujaratAgent & agent )
{
	agent.moveHome();
}

int MoveHomeAction::getTimeNeeded()
{
	return 5;
}

} // namespace Gujarat

