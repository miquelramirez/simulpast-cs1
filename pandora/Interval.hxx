#ifndef __INTERVAL_HXX_
#define __INTERVAL_HXX_

#include <iostream>

namespace Engine
{

template <typename Type> class Interval
{
public:
	
	Type _min;
	Type _max;
	
	Interval() : _min(0), _max(0)
	{
	}
	
	Interval(Type min, Type max) : _min(min), _max(max)
	{
	}	
	
	inline bool isInside(Type x)
	{
		return (x>=_min) && (x<=_max);
	}
	
	bool intersection(Interval other, Interval & result)
	{			
		Type left;
		if(_min >= other._min)
		{
			left = _min;
		}
		else
		{
			left = other._min;
		}
		
		if(!(isInside(left) && other.isInside(left)))
		{
			return false;
		}
		
		Type right;
		if(_max <= other._max)
		{
			right = _max;
		}
		else
		{
			right = other._max;
		}
		
		if(!(isInside(right) && other.isInside(right)))
		{
			return false;
		}
	
	
		result._min = left;
		result._max = right;
	
		return true;	
	}
	
	
	friend std::ostream & operator<<( std::ostream & stream, Interval<Type> & intv )
	{
		return stream << "[" << intv._min << ".." << intv._max << "]";
	}
	
};

}

#endif // Interval.hxx
