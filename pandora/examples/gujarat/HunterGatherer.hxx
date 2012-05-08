
#ifndef __HunterGatherer_hxx__
#define __HunterGatherer_hxx__

#include "GujaratAgent.hxx"

namespace Gujarat
{
class Sector;
class ForageAction;

class HunterGatherer : public GujaratAgent
{	
	int 					_surplusForReproductionThreshold; // MpiAttribute
	int 					_surplusWanted; // MpiAttribute
	int 					_homeRange; // MpiAttribute
	int 					_numSectors; // MpiAttribute
	std::vector< std::vector< int > >	_sectorsMask;

	void updateKnowledge();

	// this method checks whether P1 and P2 are in the same side of the line that goes from B to C
	bool sameSide( Engine::Point2D<int> P1, Engine::Point2D<int> P2, Engine::Point2D<int> A, Engine::Point2D<int> B );
	bool insideTriangle( Engine::Point2D<int> p,  Engine::Point2D<int> b, Engine::Point2D<int> c ); 
	
	void selectActions();
	
	void serializeAdditionalAttributes();
	
	GujaratAgent * createNewAgent();

	std::vector<Sector *> _sectors;
	std::vector<ForageAction* > _availableForage;

public:
	HunterGatherer( const std::string & id );
	virtual ~HunterGatherer();

	void setHomeRange( int v ) { _homeRange = v; }
	int  getHomeRange() const { return _homeRange; }
	void setNumSectors( int v ) { _numSectors = v; }
	int  getNumSectors() const { return _numSectors; }
	std::vector<Sector *>&	getSectors() { return _sectors; }

	void setSurplusForReproductionThreshold( int v ) { _surplusForReproductionThreshold = v; }
	void setSurplusWanted( int v ) { _surplusWanted = v; }
	bool cellValid( Engine::Point2D<int>& loc );
	bool cellRelevant( Engine::Point2D<int>& loc );
	bool needsResources();

	void		updateResources( int v ) { _collectedResources += v; }
	void 		createSectorsMask();

	void		updateKnowledge( 	const Engine::Point2D<int>& agentPos,
						const Engine::Raster& dataRaster,
						std::vector< Sector* >& sectors ) const;

	
	// MPI Script Generated code
	HunterGatherer( void * );
	void * fillPackage();
};

} // namespace Gujarat

#endif // __HunterGatherer_hxx__

