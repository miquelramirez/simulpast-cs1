#ifndef __RAMIREZ_DEMOGRAPHICS__
#define __RAMIREZ_DEMOGRAPHICS__

#include "GujaratDemographics.hxx"

namespace Gujarat
{

class RamirezDemographics : public GujaratDemographics
{
public:

	RamirezDemographics( GujaratAgent& agent );
	virtual	~RamirezDemographics();
	
	virtual bool checkEmigration();
	virtual void checkMortality();
	virtual void checkReproduction();
};


}

#endif // RamirezDemographics.hxx
