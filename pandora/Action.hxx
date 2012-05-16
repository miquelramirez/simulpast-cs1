
#ifndef __Action_hxx__
#define __Action_hxx__

#include <string>

namespace Engine
{
class Agent;

class Action
{
public:
	Action();
	virtual ~Action();
	virtual void execute( Agent & agent ) = 0;
	virtual std::string describe() const;
};

} // namespace Engine

#endif // __Action_hxx__

