#include "GujaratConfig.hxx"
#include <sstream>
#include "Exceptions.hxx"

namespace Gujarat
{

GujaratConfig::GujaratConfig() : _size(0), _soilFile("no loaded file"), _demFile("no loaded file"), _climateSeed(1)
{
}
  
GujaratConfig::GujaratConfig(const std::string & filename) : _size(0), _soilFile(""), _climateSeed(1)
{     
//    Config::_path      = (char*)0;
//    Config::_numAgents = 0;
//    Config::_numSteps  = 0;
    
    //deserializeTemplate(this, filename); 
    //deserialize(filename); 
};

GujaratConfig::~GujaratConfig()
{
}

void GujaratConfig::extractParticularAttribs(TiXmlElement * root)
{
	TiXmlElement * element = root->FirstChildElement("climateSeed");
	_climateSeed = atoi(element->Attribute("value"));
	
	element = root->FirstChildElement("soil");	
	parseSoilInfo(element);

	element = root->FirstChildElement("dem");
	_demFile = element->Attribute("fileName");

	element = root->FirstChildElement("rainHistoricalDistribution");
	_rainHistoricalDistribShape = atof(element->Attribute("shape"));
	_rainHistoricalDistribScale = atof(element->Attribute("scale"));
	_rainHistoricalDistribMean = atof(element->Attribute("mean"));

	element = root->FirstChildElement("socialRange");
	_socialRange = atoi( element->Attribute("value") ); 
	
	element = root->FirstChildElement("hunterGatherers");
	_numHG = atoi(element->Attribute("num"));
	_homeRange = atoi( element->Attribute("homeRange") );
	_surplusForReproductionThreshold = atoi( element->Attribute("surplusForReproductionThreshold") );
	_surplusWanted = atoi( element->Attribute("surplusWanted") );
	_numSectors = atoi( element->Attribute("numSectors") );

	// MRJ: Loading agro pastoralists attributes	
	element = root->FirstChildElement("agroPastoralists");
	_numAP = atoi(element->Attribute("num"));
	_maxCropHomeDistance = atoi(element->Attribute("maxCropHomeDistance"));


	element = root->FirstChildElement("daysPerSeason");
	_daysPerSeason = atoi( element->Attribute("value") );
	element = root->FirstChildElement("cellResolution");
	_cellResolution = atof( element->Attribute("value") );

	element = root->FirstChildElement("massToEnergyRate" );
	_massToEnergyRate = atof( element->Attribute("value") );
	element = root->FirstChildElement("energyToCaloriesRate" );
	_energyToCalRate = atof( element->Attribute("value") );
	
	TiXmlNode* n = NULL;
	while ( ( n = root->IterateChildren( n ) ) )
	{
		if ( n->Type() != TiXmlNode::TINYXML_ELEMENT ) continue;
		TiXmlElement* elem = n->ToElement();
		if ( elem->ValueStr().compare("cellBiomass") )
			continue;
		std::string elemType = elem->Attribute("type");
		if ( !elemType.compare("dune" ) )
		{
			_duneBiomass = atof( elem->Attribute("mean") );
			_duneBiomassStdDev = atof( elem->Attribute("stddev") );
			_duneEfficiency = atof( elem->Attribute("efficiency"));	
		}
		else if ( !elemType.compare("interdune") )
		{
			_interduneBiomass = atof( elem->Attribute("mean") );
			_interduneBiomassStdDev = atof( elem->Attribute("stddev"));
			_interduneEfficiency = atof( elem->Attribute("efficiency"));
		}
		else
		{
			std::stringstream sstr;
			sstr << "ERROR: Loading simulation config document" << std::endl;
			sstr << "Unknown cellBiomass type " << elemType << " found!!!" << std::endl;
			throw Engine::Exception( sstr.str() ); 
		}
	}

	std::cout << "[CONFIG]: Mass To Energy Rate: " << _massToEnergyRate << std::endl;
	std::cout << "[CONFIG]: Energy To Calories Rate: " << _energyToCalRate << std::endl;
	std::cout << "[CONFIG]: Dune Cell: Biomass: Mass: " << _duneBiomass << std::endl;
	std::cout << "[CONFIG]: Dune Cell: Biomass: Std. Dev: " << _duneBiomassStdDev << std::endl;
	std::cout << "[CONFIG]: Dune Cell: Biomass: Efficiency: " << _duneEfficiency << std::endl;
	std::cout << "[CONFIG]: Interdune Cell: Biomass: Mass: " << _interduneBiomass << std::endl;
	std::cout << "[CONFIG]: Interdune Cell: Biomass: Std. Dev: " << _interduneBiomassStdDev << std::endl;
	std::cout << "[CONFIG]: Interdune Cell: Biomass: Efficiency: " << _interduneEfficiency << std::endl;

}
  
void GujaratConfig::parseSoilInfo( TiXmlElement * element )
{    
	// TODO use STL
    //*? use SWITCH
	_soilFile = element->Attribute("fileName"); // path
	_size = atoi(element->Attribute("size"));
}

const int & GujaratConfig::getSize() const
{
	return _size;
}

} // namespace Gujarat

