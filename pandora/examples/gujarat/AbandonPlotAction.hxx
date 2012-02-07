
#ifndef __AbandonPlotAction_hxx__
#define __AbandonPlotAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class AbandonPlotAction : public Action
{
public:
	AbandonPlotAction();
	virtual ~AbandonPlotAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __AbandonPlotAction_hxx__

