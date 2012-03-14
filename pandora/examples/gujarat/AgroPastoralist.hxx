
#ifndef __AgroPastoralist_hxx__
#define __AgroPastoralist_hxx__

#include "GujaratAgent.hxx"

namespace Gujarat
{

class CultivatedField;

class AgroPastoralist : public GujaratAgent
{
	int _maxCropHomeDistance;
	
	void updateKnowledge();

	void evaluateYearlyActions();
	void evaluateSeasonalActions();
	void evaluateIntraSeasonalActions();
	
	void serializeAdditionalAttributes();
	
	GujaratAgent * createNewAgent();

	CultivatedField * _cultivatedField;
public:
	AgroPastoralist( const std::string & id );
	virtual ~AgroPastoralist();

	void * createPackage();

	void moveHome();
	void establishPlot();
	void sow();
	void maintainPlot();
	void harvest();
	void abandonPlot();
	void acquireCultivatedField( Engine::Point2D<int> p );
	void abandonCultivatedField();
	bool hasCultivatedField() { return _cultivatedField != NULL; }
	bool cultivatedFieldOutOfReach();

	// MPI Script Generated code
	AgroPastoralist( void * );
	void * fillPackage();

	void	setMaxCropHomeDistance( int v ) { _maxCropHomeDistance = v; }
	int	getMaxCropHomeDistance() const { return _maxCropHomeDistance; }
	bool	isColdDrySeason();

};

} // namespace Gujarat

#endif // __AgroPastoralist_hxx__

