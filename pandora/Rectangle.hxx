
#ifndef __Rectangle_hxx__
#define __Rectangle_hxx__

#include <sstream>
#include <cmath>

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
		if(_max >= other._max)
		{
			left = _max;
		}
		else
		{
			left = other._max;
		}
		
		if(!(isInside(left) && other.isInside(left)))
		{
			return false;
		}
	
		Type right;
		if(_min <= other._min)
		{
			right = _min;
		}
		else
		{
			right = other._min;
		}
	
		if(!(isInside(right) && other.isInside(right)))
		{
			return false;
		}
	
		result._min = left;
		result._max = right;
	
	return true;	
	}
	
};
	
	
// 2D Rectangle
template <typename Type> class Rectangle
{
public:
	// top left point
	Point2D<Type> _origin;
	Point2D<Type> _size;

	Rectangle() : _origin(-1,-1), _size(-1,-1)
	{
	}

	Rectangle( const Point2D<Type> & origin, const Point2D<Type> & size ) : _origin(origin), _size(size)
	{
	}

	Rectangle( const Interval<Type> & OX, const Interval<Type> & OY)
	{		
		this->intervals2Rectangle( OX, OY);		
	}	
	
	bool isInside( const Point2D<Type> & point ) const
	{
		if(point._x < _origin._x || point._x >= _origin._x+_size._x)
		{
			return false;
		}
		if(point._y < _origin._y || point._y >= _origin._y+_size._y)
		{
			return false;
		}
		return true;
	}

	void intervals2Rectangle( const Interval<Type> & iOX, const Interval<Type> & iOY)
	{		
		// Point2D origin = Point2D(iOX._min,iOY._min);		
		_origin._x = iOX._min;
		_origin._y = iOY._min;
		
		// Point2D size = Point2D(iOX._max – iOX._min +1, iOY._max – iOY._min +1);	
		_size._x = iOX._max - iOX._min +1;
		_size._y = iOY._max - iOY._min +1;		
		
	}	
	
	bool intersection(const Rectangle<Type> other, Rectangle<Type> & result)
	{	
		// Interval caracterization for Rectangle "this"
		Interval<Type> R1_OX(_origin._x,_origin._x + _size._x -1);		
		Interval<Type> R1_OY(_origin._y,_origin._y + _size._y -1);
		
		// Interval caracterization for Rectangle "other"
		Interval<Type> R2_OX(other._origin._x,other._origin._x + other._size._x -1);		
		Interval<Type> R2_OY(other._origin._y,other._origin._y + other._size._y -1);	
		
		Interval<Type> Intersection_OX;
		if (! R1_OX.intersection(R2_OX,Intersection_OX) )
		{
			return false;
		}
		
		Interval<Type> Intersection_OY;
		if (! R1_OY.intersection(R2_OY,Intersection_OY) )
		{
			return false;
		}
		
		result.intervals2Rectangle(Intersection_OX,Intersection_OY);
		
	return true;	
	}
	
	
	friend std::ostream & operator<<( std::ostream & stream, Rectangle<Type> & rectangle )
	{
		return stream << "rect:" << rectangle._origin << "->" << rectangle._origin + rectangle._size - 1;
	}

	Rectangle<Type> & operator=( const Rectangle<Type> & rectangle)
	{
		_origin = rectangle._origin;
		_size = rectangle._size;
		return *this;
	}
};

} // namespace Engine

#endif // __Rectangle_hxx__

