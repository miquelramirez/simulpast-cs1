
#ifndef __HunterGatherer_hxx__
#define __HunterGatherer_hxx__

#include "GujaratAgent.hxx"

namespace Gujarat
{

class HunterGatherer : public GujaratAgent
{	
	int _surplusForReproductionThreshold;
	int _surplusWanted;
	int _homeRange;
	
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
	
	void setHomeRange( int v ) { _homeRange = v; }
	int  getHomeRange() const { return _homeRange; }

	void setSurplusForReproductionThreshold( int v ) { _surplusForReproductionThreshold = v; }
	void setSurplusWanted( int v ) { _surplusWanted = v; }
	bool cellValid( Engine::Point2D<int>& loc );
	bool cellRelevant( Engine::Point2D<int>& loc );
	bool needsResources();

	void		updateResources( int v ) { _collectedResources += v; }
};

} // namespace Gujarat

#endif // __HunterGatherer_hxx__

