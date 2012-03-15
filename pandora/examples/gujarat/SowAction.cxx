
#include "SowAction.hxx"
#include "GujaratAgent.hxx"
#include "AgroPastoralist.hxx"
#include "CultivatedField.hxx"

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
	if ( !agroPastoralist.getCultivatedField().isDomesticated() )
		return;
	agroPastoralist.getCultivatedField().sow();
 	std::cout << "DEBUG: Agent " << agent.getId() << " executing Sow action..." << std::endl;
}

int SowAction::getTimeNeeded()
{
	return 5;
}
} // namespace Gujarat

