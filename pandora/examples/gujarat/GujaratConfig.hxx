
#ifndef __GujaratConfig_hxx__
#define __GujaratConfig_hxx__

#include "Config.hxx"
#include <tinyxml.h>
#include <string>
#include <sstream>

namespace Gujarat
{
    class GujaratWorld;

class GujaratConfig : public Config
{ 
	int 		_size;		
	std::string 	_soilFile;
	std::string 	_demFile;
	int		_climateSeed;
	// Agent home range expressed in # of underlying GIS
	// data grid (~30m)
	int		_homeRange;
	// Agents social range expressed in # GIS data grid tiles
	int		_socialRange;
	// # days corresponding to a climate model season
	int		_daysPerSeason;

	float _rainHistoricalDistribShape;
	float _rainHistoricalDistribScale;
	float _rainHistoricalDistribMean;

	int _numHG;
	int _numAP;

	void parseSoilInfo(TiXmlElement * element);
public:
	GujaratConfig();  
	GujaratConfig(const std::string & filename);  
	virtual ~GujaratConfig();
    
	void extractParticularAttribs(TiXmlElement *pRoot);
	friend std::ostream & operator<<( std::ostream & stream, const GujaratConfig & config )
	{
		return stream << "Config(" << "path:" << config._path << "," << "size:" << config._size << "," << "steps:" << config._numSteps << " soil file: " << config._soilFile << ")";
	}  
	const int & getSize() const;
	friend class GujaratWorld;
	friend class Climate;
};

} // namespace Gujarat

#endif // __GujaratConfig_hxx__

