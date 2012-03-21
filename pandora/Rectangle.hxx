
#ifndef __Rectangle_hxx__
#define __Rectangle_hxx__

#include <sstream>
#include <cmath>

#include <iostream>
#include <cstdlib>

#include "Interval.hxx" 

namespace Engine
{

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
		
		std::cout << "intev2Rect:"<<  iOX._min << "," <<  iOY._min << "|" <<  iOX._max << "," <<  iOY._max << std::endl;
	}	
	
	bool intersection(const Rectangle<Type> other, Rectangle<Type> & result)
	{	
		// Interval caracterization for Rectangle "this"
		Interval<Type> R1_OX(_origin._x,_origin._x + _size._x -1);		
		Interval<Type> R1_OY(_origin._y,_origin._y + _size._y -1);
		
		// Interval caracterization for Rectangle "other"
		Interval<Type> R2_OX(other._origin._x,other._origin._x + other._size._x -1);		
		Interval<Type> R2_OY(other._origin._y,other._origin._y + other._size._y -1);	

		if(_origin._y <0)
		{
			std::cout << "NEGATIU" << std::endl;
		}
		
std::cout << R1_OX << R1_OY	<< "&&" <<R2_OX << R2_OY << "?" << std::endl;
		
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
		
std::cout << Intersection_OX << Intersection_OY << std::endl;

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

