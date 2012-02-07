
#ifndef __HunterGatherer_hxx__
#define __HunterGatherer_hxx__

#include "GujaratAgent.hxx"

namespace Gujarat
{

class HunterGatherer : public GujaratAgent
{	
	int _surplusForReproductionThreshold;
	int _surplusWanted;
	
	void updateKnowledge();
	
	void evaluateYearlyActions();
	void evaluateSeasonalActions();
	void evaluateIntraSeasonalActions();
	
	void serializeAdditionalAttributes();
	
	GujaratAgent * createNewAgent();

public:
	HunterGatherer( const std::string & id );
	virtual ~HunterGatherer();

	void * createPackage();

	// actions
	void hunt();
	void gather();
};

} // namespace Gujarat

#endif // __HunterGatherer_hxx__

