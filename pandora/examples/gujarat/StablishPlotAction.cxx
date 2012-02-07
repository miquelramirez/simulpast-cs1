
#include "StablishPlotAction.hxx"
#include "GujaratAgent.hxx"
#include "AgroPastoralist.hxx"

namespace Gujarat
{

StablishPlotAction::StablishPlotAction()
{
}

StablishPlotAction::~StablishPlotAction()
{
}

void StablishPlotAction::execute( GujaratAgent & agent )
{
	AgroPastoralist & agroPastoralist = (AgroPastoralist&)agent;
	agroPastoralist.stablishPlot();
}

int StablishPlotAction::getTimeNeeded()
{
	return 5;
}
} // namespace Gujarat

