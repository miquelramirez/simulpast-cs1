
#include "HuntAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

HuntAction::HuntAction()
{
}

HuntAction::~HuntAction()
{
}

void HuntAction::execute( GujaratAgent & agent )
{
	HunterGatherer & hG = (HunterGatherer &)agent;
	hG.hunt();
}

int HuntAction::getTimeNeeded()
{
	return 1;
}


} // namespace Gujarat

