

#include "Climate.hxx"
#include "GujaratConfig.hxx"

namespace Gujarat
{

Climate::Climate( const GujaratConfig & config ) 
	: _randomGenerator(config._climateSeed), _currentSeason(HOTDRY),  
	_currentRain(0.0f),
	_uniformDistribution(_randomGenerator, boost::uniform_real <> (0,1)), 
	_config(config)
{
}

Climate::~Climate()
{
}

void Climate::step()
{
	/*
	World passes its Statistics object. This way random number management is centralized.
	*/

	if(_currentSeason == HOTWET)
	{
		float randomNumber = _uniformDistribution();
		// TODO document weibull distribution
		_currentRain = _config._rainHistoricalDistribScale*pow(-log(1.0-randomNumber),(1.0/_config._rainHistoricalDistribShape));
		std::cout << "new current rain: " << _currentRain << std::endl;
	}
}

void Climate::advanceSeason()
{
	_currentSeason = (Seasons)(((int)_currentSeason+1)%3);
}

float Climate::getMeanAnnualRain() const
{
	return _config._rainHistoricalDistribMean;
}

const float & Climate::getRain() const
{
	return _currentRain;
}

const Seasons & Climate::getSeason() const
{
	return _currentSeason;
}

} // namespace Gujarat

