
#ifndef __SowAction_hxx__
#define __SowAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class SowAction : public Action
{
public:
	SowAction();
	virtual ~SowAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __SowAction_hxx__

