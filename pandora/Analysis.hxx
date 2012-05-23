
#ifndef __Analysis_hxx__
#define __Analysis_hxx__

#include <string>
#include <vector>

#include "SimulationRecord.hxx"

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
	Analysis( const std::string & name );
	virtual ~Analysis();
	void setNumTimeSteps( int numTimeSteps );

	const std::string & getName() const;
	virtual void preProcess(){};
	virtual void postProcess(){};
	float getResult( int timeStep ) const;
	
};

class RasterAnalysis : public Analysis
{
public:
	RasterAnalysis( const std::string & name ) : Analysis(name)
	{
	}
	virtual void computeRaster( const Engine::SimulationRecord::RasterHistory & rasterHistory ) = 0;
};


class AgentAnalysis : public Analysis
{
public:
	AgentAnalysis( const std::string & name ) : Analysis(name)
	{
	}
	virtual void computeAgent( const Engine::AgentRecord & ) = 0;
};


} // namespace Analysis

#endif // __Analysis_hxx__
