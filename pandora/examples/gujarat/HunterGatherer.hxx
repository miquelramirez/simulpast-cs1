
#ifndef __HunterGatherer_hxx__
#define __HunterGatherer_hxx__

#include "GujaratAgent.hxx"

namespace Gujarat
{
class Sector;

class HunterGatherer : public GujaratAgent
{	
	int 					_surplusForReproductionThreshold;
	int 					_surplusWanted;
	int 					_homeRange;
	int 					_numSectors;
	std::vector< std::vector< int > >	_sectorsMask;

	void updateKnowledge();
	bool sameSide( Engine::Point2D<int> A, Engine::Point2D<int> B, Engine::Point2D<int> P );
	bool insideTriangle( Engine::Point2D<int> p,  Engine::Point2D<int> b, Engine::Point2D<int> c ); 
	
	void evaluateYearlyActions();
	void evaluateSeasonalActions();
	void evaluateIntraSeasonalActions();
	
	void serializeAdditionalAttributes();
	
	GujaratAgent * createNewAgent();

	std::vector<Sector *> _sectors;

public:
	HunterGatherer( const std::string & id );
	virtual ~HunterGatherer();

	void * createPackage();
	
	void setHomeRange( int v ) { _homeRange = v; }
	int  getHomeRange() const { return _homeRange; }
	void setNumSectors( int v ) { _numSectors = v; }
	int  getNumSectors() const { return _numSectors; }

	void setSurplusForReproductionThreshold( int v ) { _surplusForReproductionThreshold = v; }
	void setSurplusWanted( int v ) { _surplusWanted = v; }
	bool cellValid( Engine::Point2D<int>& loc );
	bool cellRelevant( Engine::Point2D<int>& loc );
	bool needsResources();

	void		updateResources( int v ) { _collectedResources += v; }
	void 		createSectorsMask();
};

} // namespace Gujarat

#endif // __HunterGatherer_hxx__

