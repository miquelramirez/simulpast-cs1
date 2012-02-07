
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
	void stablishPlot();
	void sow();
	void maintainPlot();
	void harvest();
	void abandonPlot();
};

} // namespace Gujarat

#endif // __AgroPastoralist_hxx__

