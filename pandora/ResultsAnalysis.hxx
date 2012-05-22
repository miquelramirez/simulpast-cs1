
#ifndef __ResultsAnalysis_hxx__
#define __ResultsAnalysis_hxx__

#include <list>
#include <string>

namespace Engine
{
	class SimulationRecord;
}

namespace Analysis
{

class Analysis;

class ResultsAnalysis
{
	typedef std::list<Analysis *> AnalysisList;
	AnalysisList _analysisList;
	const Engine::SimulationRecord & _simRecord;

	std::string _outputFile;
	std::string _agentType;
	std::string _separator;

public:
	ResultsAnalysis( Engine::SimulationRecord & simRecord, const std::string & outputFile, const std::string & agentType = "all", const std::string & separator = ";" );	
	virtual ~ResultsAnalysis();

	void apply() const;
	void addAnalysis( Analysis * analysis );
};

} // namespace Analysis

#endif // __ResultsAnalysis_hxx__

