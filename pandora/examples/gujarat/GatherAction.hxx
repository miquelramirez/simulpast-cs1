
#ifndef __GatherAction_hxx__
#define __GatherAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class GatherAction : public Action
{
public:
	GatherAction();
	virtual ~GatherAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __GatherAction_hxx__

