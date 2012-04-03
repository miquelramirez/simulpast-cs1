
#ifndef __HarvestAction_hxx__
#define __HarvestAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class HarvestAction : public Action
{
public:
	HarvestAction();
	virtual ~HarvestAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded() const;
};
	
} // namespace Gujarat

#endif // __HarvestAction_hxx__

