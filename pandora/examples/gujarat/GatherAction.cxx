
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

void GatherAction::execute( GujaratAgent & a )
{
	HunterGatherer& agent = (HunterGatherer&) a;
	if (!agent.needsResources()) return;

	Engine::Point2D<int> location;
	std::cout << "DEBUG: Gather action executing..." << std::endl;

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
			int collected = agent.getWorld()->getStatistics().getUniformDistValue( actualValue/5, actualValue/2 );  		
			agent.updateResources(collected);
			agent.getWorld()->setValue("resources", location, actualValue-collected);
		}
	}
}

int GatherAction::getTimeNeeded()
{
	return 1;
}


} // namespace Gujarat

