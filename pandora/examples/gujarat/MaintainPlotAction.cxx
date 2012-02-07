
#include "MaintainPlotAction.hxx"
#include "GujaratAgent.hxx"
#include "AgroPastoralist.hxx"

namespace Gujarat
{

MaintainPlotAction::MaintainPlotAction()
{
}

MaintainPlotAction::~MaintainPlotAction()
{
}

void MaintainPlotAction::execute( GujaratAgent & agent )
{
	AgroPastoralist & agroPastoralist = (AgroPastoralist&)agent;
	agroPastoralist.maintainPlot();
}

int MaintainPlotAction::getTimeNeeded()
{
	return 5;
}
} // namespace Gujarat

