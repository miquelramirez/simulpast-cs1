
#ifndef __HuntAction_hxx__
#define __HuntAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class HuntAction : public Action
{
public:
	HuntAction();
	virtual ~HuntAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __HuntAction_hxx__

