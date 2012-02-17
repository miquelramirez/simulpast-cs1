
#include "HuntAction.hxx"
#include "GujaratAgent.hxx"
#include "HunterGatherer.hxx"
#include "Point2D.hxx"

namespace Gujarat
{

HuntAction::HuntAction()
{
}

HuntAction::~HuntAction()
{
}

void HuntAction::execute( GujaratAgent & a )
{
	HunterGatherer& agent = (HunterGatherer&) a;
	if (!agent.needsResources()) return;

	Engine::Point2D<int> location;
	std::cout << "H";

	const Engine::Point2D<int>& pos = agent.getPosition();
	int   hr = agent.getHomeRange();
	
	for( location._x=pos._x- hr; 
	     location._x <= pos._x+ hr; location._x++)
	{
		for( location._y=pos._y - hr; 
			location._y<=pos._y + hr ; location._y++)
		{
			if ( !agent.cellValid(location) ) continue;
			if ( !agent.cellRelevant(location) ) continue;

			// Action execution (modifies the world)
			int actualValue = agent.getWorld()->getValue("resources", location);
			int collected = agent.getWorld()->getStatistics().getUniformDistValue(0,actualValue);  		
			agent.updateResources(collected);
			agent.getWorld()->setValue("resources", location, actualValue-collected);
		}
	}
	
	std::cout << "***********************" << std::endl;

}

int HuntAction::getTimeNeeded()
{
	return 1;
}


} // namespace Gujarat

