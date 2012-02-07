
#ifndef __AgentRecord_hxx__
#define __AgentRecord_hxx__

#include <map>
#include <vector>
#include <string>

namespace Engine
{

class AgentRecord
{
	typedef std::vector<int> ValuesVector;
	std::string _id;
public:
	typedef std::map<std::string, ValuesVector > StatesMap;
	// total number of steps in simulation (not agent life)
	int _numSteps;
	StatesMap _states;
public:
	AgentRecord( const std::string & id, int numSteps );
	virtual ~AgentRecord();
	// TODO x and y must be integers or strings?
	void addState( int numStep, const std::string & id, int value );
	int getState( int numStep, const std::string & key );
	std::string getCompleteState( int numStep );

	StatesMap::iterator beginStates();
	StatesMap::iterator endStates();
};

} // namespace Engine

#endif // __AgentRecord_hxx__

