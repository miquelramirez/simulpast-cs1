
#ifndef __APMoveHomeAction_hxx__
#define __APMoveHomeAction_hxx__

#include "Action.hxx"
#include "Point2D.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class APMoveHomeAction : public Action
{
	Engine::Point2D<int>	_newHomeLoc;

public:
	APMoveHomeAction( Engine::Point2D<int>& p );
	virtual ~APMoveHomeAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded();

	static void	generatePossibleActions( GujaratAgent& agent, std::vector< APMoveHomeAction* >& actions );
};
	
} // namespace Gujarat

#endif // __APMoveHomeAction_hxx__

