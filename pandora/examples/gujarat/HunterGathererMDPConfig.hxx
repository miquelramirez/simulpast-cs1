#ifndef __HunterGathererMDPConfig_hxx__
#define __HutnerGathererMDPConfig_hxx__

#include <iosfwd>

class TiXmlElement;

namespace Gujarat
{

class HunterGathererMDPConfig
{
public:
	HunterGathererMDPConfig( TiXmlElement* elem );
	~HunterGathererMDPConfig();	


	int	getNumberForageActions() const { return _nrForageActions; }
	int	getNumberMoveHomeActions() const { return _nrMoveHomeActions; }
	bool	isDoNothingAllowed() const { return _doNothingAllowed; } 
	int	getHorizon() const { return _horizon; } 

	void	dump( std::ostream& os ) const;

private:

	int	_nrForageActions;
	int	_nrMoveHomeActions;
	bool	_doNothingAllowed;
	int	_horizon;

};

}

#endif // HunterGathererMDPConfig.hxx
