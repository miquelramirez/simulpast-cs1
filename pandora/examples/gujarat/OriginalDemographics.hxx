#ifndef __ORIGINAL_DEMOGRAPHICS__
#define __ORIGINAL_DEMOGRAPHICS__

#include "GujaratDemographics.hxx"

namespace Gujarat
{

class OriginalDemographics : public GujaratDemographics
{
public:

	OriginalDemographics( GujaratAgent& agent );
	virtual	~OriginalDemographics();
	
	virtual bool checkEmigration();
	virtual void checkMortality();
	virtual void checkReproduction();
};

}

#endif // OriginalDemographics.hxx
