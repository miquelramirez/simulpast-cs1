
#include "RasterConfiguration.hxx"
#include "ColorSelector.hxx"
#include "DefaultColorSelector.hxx"
#include <iostream>

namespace GUI
{

RasterConfiguration::RasterConfiguration( const int & minValue, const int & maxValue) : _colorSelector(0), _minValue(minValue), _maxValue(maxValue), _transparentEnabled(false), _transparentValue(0)
{
	resetColorRamp();
	
	// basic behavior, 5 different random values
	int numSteps = std::min(5, maxValue+1-_minValue);
	float range = (float)(_maxValue + 1 - _minValue)/numSteps;
	float rangeColor = 255.0f/(numSteps-1);
	
	std::cout << "num steps: " << numSteps << " min/max: " << minValue << "/" << maxValue << " range: " << range << " and range color: " << rangeColor << std::endl;
	// first color = grey, not black
	_colorSelector->setColor(QColor(100,100,100), 0);
	for(int i=1; i<numSteps; i++)
	{
		_colorSelector->addBreak(minValue+range*i);
		//std::cout << "\tadding break: " << minValue+range*i << " with color: " << rangeColor*i << std::endl;
		_colorSelector->setColor(QColor(0,rangeColor*i,0), i);
	}

	/*
	for(int i=0; i<_colorSelector->getNumIntervals(); i++)
	{
		std::cout << "i: " << i << " from: " << _colorSelector->getBreak(i) << " to: " << _colorSelector->getBreak(i+1)-1 << " with color: " << _colorSelector->getColor(_colorSelector->getBreak(i)).name().toStdString() << std::endl;
	}
	*/
}

RasterConfiguration::RasterConfiguration( const RasterConfiguration & prototype ) : _colorSelector(0), _minValue(prototype.getMinValue()), _maxValue(prototype.getMaxValue()), _transparentEnabled(prototype.isTransparentEnabled()), _transparentValue(prototype.getTransparentValue())
{
	_colorSelector =  prototype.getColorRamp().copy();
	
	/*
	for(int i=0; i<_colorSelector->getNumIntervals(); i++)
	{
		std::cout << "copy i: " << i << " from: " << _colorSelector->getBreak(i) << " to: " << _colorSelector->getBreak(i+1)-1 << " with color: " << _colorSelector->getColor(_colorSelector->getBreak(i)).name().toStdString() << std::endl;
	}
	*/
}

RasterConfiguration::~RasterConfiguration()
{
	delete _colorSelector;
}

void RasterConfiguration::resetColorRamp()
{
	if(_colorSelector)
	{
		delete _colorSelector;
	}
	_colorSelector = new DefaultColorSelector(_minValue,_maxValue);
}

ColorSelector & RasterConfiguration::getColorRamp()
{
	return *_colorSelector;
}

const ColorSelector & RasterConfiguration::getColorRamp() const
{
	return *_colorSelector;
}

const int & RasterConfiguration::getMinValue() const
{
	return _minValue;
}

const int & RasterConfiguration::getMaxValue() const
{
	return _maxValue;
}

const bool & RasterConfiguration::isTransparentEnabled() const
{
	return _transparentEnabled;
}

void RasterConfiguration::setTransparentEnabled( const bool & transparentEnabled )
{
	_transparentEnabled = transparentEnabled;
}

const int & RasterConfiguration::getTransparentValue() const
{
	return _transparentValue;
}

void RasterConfiguration::setTransparentValue( const int & transparentValue )
{
	_transparentValue = transparentValue;
}

} // namespace GUI

