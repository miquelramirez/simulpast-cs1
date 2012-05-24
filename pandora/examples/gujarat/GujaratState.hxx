
#ifndef __GujaratState_hxx__
#define __GujaratState_hxx__


#include <string>
#include "GeneralState.hxx"

class TiXmlElement;

namespace Gujarat 
{

class CaloricRequirementsTable;
class GujaratDemographics;

class GujaratState
{
	static GujaratState * _instance;

	static CaloricRequirementsTable * _hgCaloricRequirements;
	static CaloricRequirementsTable * _apCaloricRequirements;

	static GujaratDemographics * _demographics;

protected:
	GujaratState();

public:
	static GujaratState & instance();
	virtual ~GujaratState();

	static void setHGCaloricRequirements( TiXmlElement * element );
	static void setAPCaloricRequirements( TiXmlElement * element );
	static void setDemographics( const std::string & model );

	static CaloricRequirementsTable & caloricRequirements( const std::string & type );
	static GujaratDemographics & demographics();

};

} // namespace Gujarat

#endif // __GujaratState_hxx__

