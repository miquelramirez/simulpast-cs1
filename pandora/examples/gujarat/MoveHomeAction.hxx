
#ifndef __MoveHomeAction_hxx__
#define __MoveHomeAction_hxx__

#include "Action.hxx"
#include "Point2D.hxx"

namespace Gujarat
{

class GujaratAgent;
	
class MoveHomeAction : public Action
{
	Engine::Point2D<int>	_newHomeLoc;

public:
	MoveHomeAction( Engine::Point2D<int>& p );
	virtual ~MoveHomeAction();
	// TODO templatitzar classe per agent
	void execute( GujaratAgent & agent );
	int getTimeNeeded() const;

	static void	generatePossibleActions( GujaratAgent& agent, std::vector< MoveHomeAction* >& actions );
	static void	generatePossibleActions( const GujaratAgent& agent,
						const Engine::Point2D<int>& agentPos,
						std::vector< MoveHomeAction* >& actions );
};
	
} // namespace Gujarat

#endif // __MoveHomeAction_hxx__

