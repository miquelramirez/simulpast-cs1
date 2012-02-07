
#ifndef __StablishPlotAction_hxx__
#define __StablishPlotAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class StablishPlotAction : public Action
{
public:
	StablishPlotAction();
	virtual ~StablishPlotAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __StablishPlotAction_hxx__

