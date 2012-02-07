
#include "HarvestAction.hxx"
#include "GujaratAgent.hxx"
#include "AgroPastoralist.hxx"

namespace Gujarat
{

HarvestAction::HarvestAction()
{
}

HarvestAction::~HarvestAction()
{
}

void HarvestAction::execute( GujaratAgent & agent )
{
	AgroPastoralist & agroPastoralist = (AgroPastoralist&)agent;
	agroPastoralist.harvest();
}

int HarvestAction::getTimeNeeded()
{
	return 5;
}
} // namespace Gujarat

