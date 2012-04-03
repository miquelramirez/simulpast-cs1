#ifndef __DoNothingAction_hxx__
#define __DoNothingAction_hxx__

#include "Action.hxx"
#include <vector>

namespace Gujarat
{

class GujaratAgent;

class DoNothingAction : public Action
{
public:
	DoNothingAction();
	virtual ~DoNothingAction();

	void 	execute( GujaratAgent& agent );
	int	getTimeNeeded() const { return 0; }

	static	void	generatePossibleActions( GujaratAgent& agent, std::vector< DoNothingAction* >& actions );
	
	virtual void execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const;

protected:
	
};

}

#endif // DoNothingAction.hxx
