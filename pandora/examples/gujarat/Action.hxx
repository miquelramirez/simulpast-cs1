

#ifndef __Action_hxx__
#define __Action_hxx__

namespace Gujarat
{
class GujaratAgent;
	
class Action
{
public:
	friend class GujaratAgent;
	Action();
	virtual ~Action();
	virtual void execute( GujaratAgent & agent ) = 0;
	virtual int getTimeNeeded() = 0;
};
	
} // namespace Gujarat

#endif // __Action_hxx__

