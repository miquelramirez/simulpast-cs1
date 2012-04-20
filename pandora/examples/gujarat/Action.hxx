#ifndef __Action_hxx__
#define __Action_hxx__

#include <cstdlib>
#include <iosfwd>

namespace Gujarat
{
class 	GujaratAgent;
class 	HunterGathererMDPState;

class Action
{
public:
	friend class GujaratAgent;
	Action();
	virtual ~Action();
	virtual void execute( GujaratAgent & agent ) = 0;
	virtual void execute( const GujaratAgent& agent, const HunterGathererMDPState& s, HunterGathererMDPState& sp ) const;

	virtual int getTimeNeeded() const = 0;

	virtual Action*	copy() const { return NULL; }
	virtual void	describe( std::ostream& os ) const;
};
	
} // namespace Gujarat

#endif // __Action_hxx__

