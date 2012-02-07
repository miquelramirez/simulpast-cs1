
#ifndef __MaintainPlotAction_hxx__
#define __MaintainPlotAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class MaintainPlotAction : public Action
{
public:
	MaintainPlotAction();
	virtual ~MaintainPlotAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __MaintainPlotAction_hxx__

