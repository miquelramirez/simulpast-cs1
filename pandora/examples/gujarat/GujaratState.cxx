
#include <GujaratState.hxx>

#include <tinyxml.h>
#include <Exceptions.hxx>
#include <CaloricRequirementsTable.hxx>
#include <GujaratDemographics.hxx>
#include <RamirezDemographics.hxx>
#include <AlexisDemographics.hxx>
#include <OriginalDemographics.hxx>

namespace Gujarat 
{

GujaratState * GujaratState::_instance = 0;
CaloricRequirementsTable * GujaratState::_hgCaloricRequirements = 0;
CaloricRequirementsTable * GujaratState::_apCaloricRequirements = 0;
GujaratDemographics * GujaratState::_demographics = 0;

GujaratState & GujaratState::instance()
{
	if(!_instance)
	{
		_instance = new GujaratState;
	}
	return *_instance;
}

GujaratState::GujaratState()
{
}

GujaratState::~GujaratState()
{
	if ( _hgCaloricRequirements )		
	{
		delete _hgCaloricRequirements;
		_hgCaloricRequirements = 0;
	}
	if ( _apCaloricRequirements )
	{
		delete _apCaloricRequirements;
		_apCaloricRequirements = 0;
	}
	if(_demographics)
	{
		delete _demographics;
		_demographics = 0;
	}
}

void GujaratState::setHGCaloricRequirements( TiXmlElement * element )
{
	if(_hgCaloricRequirements)
	{
		delete _hgCaloricRequirements;
	}
	_hgCaloricRequirements = new CaloricRequirementsTable(element);
}

void GujaratState::setAPCaloricRequirements( TiXmlElement * element )
{
	if(_apCaloricRequirements)
	{
		delete _apCaloricRequirements;
	}
	_apCaloricRequirements = new CaloricRequirementsTable(element);
}

CaloricRequirementsTable & GujaratState::caloricRequirements( const std::string & type )
{
	if(type.compare("HunterGatherer")==0)
	{
		if(!_hgCaloricRequirements)
		{
			std::stringstream oss;
			oss << "GujaratState::caloricRequirements() - hg table not initialized";
			throw Engine::Exception(oss.str());
		}
		return *(instance()._hgCaloricRequirements);
	}
	else if(type.compare("AgroPastoralist")==0)
	{
		if(!_apCaloricRequirements)
		{
			std::stringstream oss;
			oss << "GujaratState::caloricRequirements() - ap table not initialized";
			throw Engine::Exception(oss.str());
		}
		return *(instance()._apCaloricRequirements);
	}
	std::stringstream oss;
	oss << "GujaratState::caloricRequirements() - asking for table with unknown type: " << type;
	throw Engine::Exception(oss.str());

}
	
void GujaratState::setDemographics( const std::string & model )
{
	if(_demographics)
	{
		delete _demographics;
	}

	if(model.compare("original")==0)
	{
		_demographics = new OriginalDemographics;
		return;
	}
	else if(model.compare("ramirez")==0)
	{
		_demographics = new RamirezDemographics;
		return;
	}
	else if(model.compare("alexis")==0)
	{
		_demographics = new AlexisDemographics;
		return;
	}
	
	
	std::stringstream oss;
	oss << "GujaratState::setDemographics() - unknown model: " << model;
	throw Engine::Exception(oss.str());
}

GujaratDemographics & GujaratState::demographics()
{
	if(!_demographics)
	{
		std::stringstream oss;
		oss << "GujaratState::demographics() - asking for demographics without being initialized";
		throw Engine::Exception(oss.str());
	}
	return *(instance()._demographics);
}

} // namespace Gujarat 

