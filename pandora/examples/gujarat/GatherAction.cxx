
#include "GatherAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

GatherAction::GatherAction()
{
}

GatherAction::~GatherAction()
{
}

void GatherAction::execute( GujaratAgent & agent )
{
	HunterGatherer & hunterGatherer = (HunterGatherer&)agent;
	hunterGatherer.gather();
}

int GatherAction::getTimeNeeded()
{
	return 1;
}


} // namespace Gujarat

