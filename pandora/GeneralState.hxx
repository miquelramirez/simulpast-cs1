
#ifndef __GeneralState_hxx__
#define __GeneralState_hxx__

#include "Logger.hxx"
#include "Statistics.hxx"

namespace Engine
{

class GeneralState
{
	static GeneralState * _instance;

	Logger _logger;
	//! random number generator
	Statistics _statistics;
protected:
	GeneralState();

public:
	static GeneralState & instance();
	virtual ~GeneralState();

	static Logger & logger()
	{
		return instance()._logger;
	}

	static Statistics & statistics()
	{
		return instance()._statistics;
	}
};

} // namespace Engine

#endif // __GeneralState_hxx__

