
#include "DefaultColorSelector.hxx"
#include <GL/gl.h>
#include <algorithm>
#include "Exceptions.hxx"
#include <iostream>

namespace GUI
{

DefaultColorSelector::DefaultColorSelector( const int & minValue, const int & maxValue )
{
	_intervals.push_back(minValue);
	_intervals.push_back(maxValue+1);
	_colors.push_back(QColor(255,255,255));
}

DefaultColorSelector::DefaultColorSelector( const DefaultColorSelector & prototype )
{
	_intervals.resize(prototype.getNumIntervals()+1);
	_colors.resize(prototype.getNumIntervals());

	_intervals[0] = prototype.getInterval(0);
	for(unsigned int i=0; i<prototype.getNumIntervals(); i++)
	{
		_intervals[i+1] = prototype.getInterval(i+1);
		_colors[i] = prototype.getColor(_intervals[i]);
	}
}

ColorSelector * DefaultColorSelector::copy() const
{

	return new DefaultColorSelector(*this);

}
	
const int & DefaultColorSelector::getInterval( const int & index ) const
{
	return _intervals.at(index);
}

DefaultColorSelector::~DefaultColorSelector()
{
}

void DefaultColorSelector::addBreak( const int & value)
{
	if(value<=_intervals[0] || value>=_intervals[_intervals.size()-1])
	{
		std::stringstream oss;
		oss << "DefaultColorSelector::addbreak - value: " << value << " must be > " << _intervals[0] << " and < : " << _intervals[_intervals.size()-1];
		throw Engine::Exception(oss.str());
	}
	std::vector<int>::iterator it = std::find(_intervals.begin(), _intervals.end(), value);
	if(it!=_intervals.end())
	{
		std::stringstream oss;
		oss << "DefaultColorSelector::addbreak - value: " << value << " already present";
		throw Engine::Exception(oss.str());
	}
	_intervals.push_back(value);
	// we order the vector of intervals in a increasing scale
	std::sort(_intervals.begin(), _intervals.end());

	it = std::find(_intervals.begin(), _intervals.end(), value);
	// finally we insert a new color in the ramp, that will be a half path between adjacent intervals
	std::vector<int>::difference_type index = std::distance(_intervals.begin(), it);
	//std::cout << "index: " << index << std::endl;

	QColor newColor(_colors[index-1]);
	if(index+1<_colors.size())
	{
		QColor before(_colors[index-1]);
		QColor after(_colors[index]);

		newColor = QColor((before.red()+after.red())/2, (before.green()+after.green())/2, (before.blue()+after.blue())/2);
	}
	std::vector<QColor>::iterator itC = _colors.begin()+index;	
	_colors.insert(itC, newColor);
}

const QColor & DefaultColorSelector::getColor( const int & value ) const
{
	for(unsigned int i=0; i<_intervals.size()-1; i++)
	{
		//std::cout << "checking interval from: " << _intervals[i] << " to: " << _intervals[i+1] << " for value: " << value << std::endl;
		if(_intervals[i]<=value && value<_intervals[i+1])
		{
			return _colors[i];
		}
	}
	return _colors[_colors.size()-1];	
}

unsigned int DefaultColorSelector::getNumIntervals() const
{
	return _colors.size();
}

const int & DefaultColorSelector::getMinValue() const
{
	return _intervals[0];
}

const int & DefaultColorSelector::getMaxValue() const
{
	return _intervals[_intervals.size()-1];
}
	
const int & DefaultColorSelector::getBreak( const int & index ) const
{
	return _intervals.at(index);
}
		
void DefaultColorSelector::setColor( const QColor & color, const int & index )
{
	_colors.at(index) = color;
}

} // namespace GUI

