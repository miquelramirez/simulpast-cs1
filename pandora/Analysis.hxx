
#ifndef __Analysis_hxx__
#define __Analysis_hxx__

#include <string>
#include <vector>

namespace Engine
{
	class AgentRecord;
}

namespace Analysis
{

class Analysis
{
protected:
	std::string _name;
	std::vector<float> _results;
public:
	Analysis( const std::string name );
	virtual ~Analysis();
	void setNumTimeSteps( int numTimeSteps );

	const std::string & getName() const;
	virtual void computeAgent( const Engine::AgentRecord & ) = 0;
	virtual void preProcess(){};
	virtual void postProcess(){};
	float getResult( int timeStep ) const;
	
};

} // namespace Analysis

#endif // __Analysis_hxx__
