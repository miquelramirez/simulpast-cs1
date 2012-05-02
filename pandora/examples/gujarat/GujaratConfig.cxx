#include "GujaratConfig.hxx"
#include <sstream>
#include "Exceptions.hxx"
#include "HunterGathererMDPConfig.hxx"

namespace Gujarat
{

GujaratConfig::GujaratConfig() 
	: _size(0), _soilFile("no loaded file"), _demFile("no loaded file"), _climateSeed(1),
	_hunterGathererController( "Rule-Based" ), _controllerConfig(NULL)
{
}
  
GujaratConfig::GujaratConfig(const std::string & filename) 
	: _size(0), _soilFile(""), _climateSeed(1), _hunterGathererController( "Rule-Based" ),
	_controllerConfig( NULL )
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

void GujaratConfig::retrieveAttributeMandatory( TiXmlElement* elem, std::string attrName, std::string& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		sstr << "[CONFIG]: ERROR: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		throw Engine::Exception(sstr.str());
	}
	value = *retrievedStr;
}

void GujaratConfig::retrieveAttributeOptional( TiXmlElement* elem, std::string attrName, std::string& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		std::cerr << "[CONFIG]: WARNING: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		value = "";
		return;	
	}
	value = *retrievedStr;

}

void GujaratConfig::retrieveAttributeMandatory( TiXmlElement* elem, std::string attrName, int& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		sstr << "[CONFIG]: ERROR: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		throw Engine::Exception(sstr.str());
	}
	value = atoi(retrievedStr->c_str());
}

void GujaratConfig::retrieveAttributeOptional( TiXmlElement* elem, std::string attrName, int& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		std::cerr << "[CONFIG]: WARNING: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		value = 0;
		return;	
	}
	value = atoi(retrievedStr->c_str());

}

void GujaratConfig::retrieveAttributeMandatory( TiXmlElement* elem, std::string attrName, float& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		sstr << "[CONFIG]: ERROR: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		throw Engine::Exception(sstr.str());
	}
	value = atof(retrievedStr->c_str());
}

void GujaratConfig::retrieveAttributeOptional( TiXmlElement* elem, std::string attrName, float& value )
{
	const std::string* retrievedStr = NULL;
	retrievedStr = elem->Attribute( attrName );
	if ( retrievedStr == NULL )
	{
		std::stringstream sstr;
		std::cerr << "[CONFIG]: WARNING: Attribute " << elem->ValueStr() << "." << attrName << " not found!" << std::endl;
		value = 0.0f;
		return;	
	}
	value = atof(retrievedStr->c_str());

}

void GujaratConfig::extractParticularAttribs(TiXmlElement * root)
{
	TiXmlElement * element = NULL;
	
	element = root->FirstChildElement("climateSeed");
	retrieveAttributeMandatory( element, "value", _climateSeed );
	
	element = root->FirstChildElement("soil");	
	parseSoilInfo(element);

	element = root->FirstChildElement("dem");
	retrieveAttributeMandatory( element, "fileName", _demFile );

	element = root->FirstChildElement("rainHistoricalDistribution");

	retrieveAttributeMandatory( element, "shape", _rainHistoricalDistribShape );
	retrieveAttributeMandatory( element, "scale", _rainHistoricalDistribScale );
	retrieveAttributeMandatory( element, "mean", _rainHistoricalDistribMean );

	element = root->FirstChildElement("socialRange");
	retrieveAttributeMandatory( element, "value", _socialRange );
	
	element = root->FirstChildElement("hunterGatherers");
	retrieveAttributeMandatory( element, "num", _numHG );
	retrieveAttributeMandatory( element, "homeRange", _homeRange );
	retrieveAttributeMandatory( element, "surplusForReproductionThreshold", _surplusForReproductionThreshold );
	retrieveAttributeMandatory( element, "surplusWanted", _surplusWanted );
	retrieveAttributeMandatory( element, "surplusSpoilage", _surplusSpoilage );
	retrieveAttributeMandatory( element, "numSectors", _numSectors );
	retrieveAttributeMandatory( element, "walkingSpeedHour", _walkingSpeedHour );
	retrieveAttributeMandatory( element, "forageTimeCost", _forageTimeCost );
	retrieveAttributeMandatory( element, "availableForageTime", _availableForageTime );
	retrieveAttributeMandatory( element, "controllerType", _hunterGathererController );
	
	_hunterGathererController = element->Attribute("controllerType");

	parseHGMDPConfig( element->FirstChildElement("controllerConfig") );


	// MRJ: Loading agro pastoralists attributes	
	element = root->FirstChildElement("agroPastoralists");
	retrieveAttributeMandatory( element, "num", _numAP );
	retrieveAttributeMandatory( element, "maxCropHomeDistance", _maxCropHomeDistance );

	element = root->FirstChildElement("daysPerSeason");
	retrieveAttributeMandatory( element, "value", _daysPerSeason );

	element = root->FirstChildElement("cellResolution");
	retrieveAttributeMandatory( element, "value", _cellResolution );

	element = root->FirstChildElement("massToEnergyRate" );
	retrieveAttributeMandatory( element, "value", _massToEnergyRate );
	element = root->FirstChildElement("energyToCaloriesRate" );
	retrieveAttributeMandatory( element, "value", _energyToCalRate );
	
	TiXmlNode* n = NULL;
	while ( ( n = root->IterateChildren( n ) ) )
	{
		if ( n->Type() != TiXmlNode::TINYXML_ELEMENT ) continue;
		TiXmlElement* elem = n->ToElement();
		if ( !elem->ValueStr().compare("cellBiomass") )
		{
			std::string elemType;
			retrieveAttributeMandatory( elem, "type", elemType );
			if ( !elemType.compare("dune" ) )
			{
				retrieveAttributeMandatory( elem, "mean", _duneBiomass );
				retrieveAttributeMandatory( elem, "stddev", _duneBiomassStdDev );
				retrieveAttributeMandatory( elem, "efficiency", _duneEfficiency );
			}
			else if ( !elemType.compare("interdune") )
			{
				retrieveAttributeMandatory( elem, "mean", _interduneBiomass );
				retrieveAttributeMandatory( elem, "stddev", _interduneBiomassStdDev );
				retrieveAttributeMandatory( elem, "efficiency", _interduneEfficiency );
			}
			else
			{
				std::stringstream sstr;
				sstr << "ERROR: Loading simulation config document" << std::endl;
				sstr << "Unknown cellBiomass type " << elemType << " found!!!" << std::endl;
				throw Engine::Exception( sstr.str() ); 
			}
		}
		else if ( !elem->ValueStr().compare( "storeRaster" ) ) 
		{
			std::string name, valueStr;
			retrieveAttributeMandatory( elem, "name", name );
			retrieveAttributeMandatory( elem, "value", valueStr );
			bool value = ( valueStr == "yes" ? true : false );
			_storeRasters[name] = value;
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
	std::cout << "[CONFIG]: Hunter Gatherer Controller: " << _hunterGathererController << std::endl;
	if ( _controllerConfig != NULL )
		_controllerConfig->dump( std::cout ); 

}
 
void GujaratConfig::parseHGMDPConfig( TiXmlElement* element )
{
	if ( element == NULL )
	{
		std::cerr << "[CONFIG]: controllerConfig element not found on configuration document" << std::endl;
		return;
	}
	// Agent controller is not set to be model-based action selection
	if ( _hunterGathererController != std::string("MDP") ) return;
	// Element is not a controllerConfig element
	if (  element->ValueStr().compare("controllerConfig") ) return;
	// Not model-based controller config
	if ( element->Attribute("type") != std::string("MDP") ) return;

	_controllerConfig = new HunterGathererMDPConfig(element);
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

