
#ifndef __MoveHomeAction_hxx__
#define __MoveHomeAction_hxx__

#include "Action.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class MoveHomeAction : public Action
{
public:
	MoveHomeAction();
	virtual ~MoveHomeAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();
};
	
} // namespace Gujarat

#endif // __MoveHomeAction_hxx__

