
#include "GujaratConfig.hxx"

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
	
	element = root->FirstChildElement("agroPastoralists");
	_numAP = atoi(element->Attribute("num"));
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

