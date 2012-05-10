#ifndef __GUJARAT_DEMOGRAPHICS__
#define __GUJARAT_DEMOGRAPHICS__

#include "GujaratAgent.hxx"

namespace Gujarat
{

class GujaratDemographics 
{
public:
	GujaratDemographics( GujaratAgent& agent );
	virtual ~GujaratDemographics();

	virtual bool	checkEmigration( ) = 0;
	virtual	void	checkMortality( ) = 0;
	virtual void	checkReproduction( ) = 0;	
	virtual	void	checkStarvationMortality();

protected:

	GujaratAgent& 	_agent;

};

}

#endif // GujaratDemographics.hxx
