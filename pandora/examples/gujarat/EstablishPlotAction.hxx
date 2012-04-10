
#ifndef __EstablishPlotAction_hxx__
#define __EstablishPlotAction_hxx__

#include "Action.hxx"
#include "Point2D.hxx"
#include <vector>

namespace Gujarat
{

class GujaratAgent;
	
class EstablishPlotAction : public Action
{
public:
	EstablishPlotAction( Engine::Point2D<int> p );
	virtual ~EstablishPlotAction();

	void execute( GujaratAgent & agent );
	int getTimeNeeded() const;

	static void generatePossibleActions( GujaratAgent& agent,
	std::vector< EstablishPlotAction* >& actions );

private:
	
	Engine::Point2D<int>	_plotLocation;
};
	
} // namespace Gujarat

#endif // __EstablishPlotAction_hxx__

