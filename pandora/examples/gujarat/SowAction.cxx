
#include "SowAction.hxx"
#include "GujaratAgent.hxx"
#include "AgroPastoralist.hxx"

namespace Gujarat
{

SowAction::SowAction()
{
}

SowAction::~SowAction()
{
}

void SowAction::execute( GujaratAgent & agent )
{
	AgroPastoralist & agroPastoralist = (AgroPastoralist&)agent;
	agroPastoralist.sow();
}

int SowAction::getTimeNeeded()
{
	return 5;
}
} // namespace Gujarat

