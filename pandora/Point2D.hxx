
#ifndef __Point2D_hxx__
#define __Point2D_hxx__

#include <sstream>
#include <cmath>

namespace Engine
{

//! Basic pointing object
/*!
  This class contains x/y coordinates, as well as needed utilities (arithmetics, distance, etc.)
  It is templatised by the type of coordinates
*/
template <typename Type> class Point2D
{
public:
	Type _x;
	Type _y;

	Point2D()
	{
		_x = 0;
		_y = 0;
	}

	Point2D( const Type & x, const Type & y )
	{
		_x = x;
		_y = y;
	}
	friend std::ostream & operator<<( std::ostream & stream, const Point2D<Type> & point )
	{
		return stream << point._x << "/" << point._y;
	}
	
	//! Point Comparison : Equality within an square area around the caller object.
	/*!
	The param "point" is considered equal if it falls in the rectangle ((_x-delta,_y-delta)..(_x+delta,_y+delta)).
	Assuming ((a,b)..(c,d)) a rectangle with top left corner (a,b) and bottom right corner (c,d).
	*/
	bool isEqual( const Point2D<Type> & point, const Type & delta = 0 ) const
	{
		return distance(point)<=delta;
	}

	//! Point Assignation
	Point2D<Type> & operator=( const Point2D<Type> & point)
	{
		_x = point._x;
		_y = point._y;
		return *this;
	}

	bool operator==( const Point2D<Type> & point )
	{
		return isEqual(point);
	}
	

	//! Sum coordinates of the param to the coordinates of the caller object.
	Point2D<Type> & operator+=( const Point2D<Type> & point)
	{
		_x += point._x;
		_y += point._y;
		return *this;
	}

	// global arithmethic operations
	Point2D<Type> operator+( const Type & value ) const
	{
		return Point2D<Type>(_x+value,_y+value);
	}
	Point2D<Type> operator-( const Type & value ) const
	{
		return Point2D<Type>(_x-value,_y-value);
	}
	Point2D<Type> operator*( const Type & value) const
	{
		return Point2D<Type>(_x*value,_y*value);
	}
	Point2D<Type> operator/( const Type & value) const
	{
		return Point2D<Type>(_x/value,_y/value);
	}

	// point to point operations
	Point2D<Type> operator+( const Point2D<Type> & point ) const
	{
		return Point2D<Type>(_x+point._x,_y+point._y);
	}	
	Point2D<Type> operator-( const Point2D<Type> & point ) const
	{
		return Point2D<Type>(_x-point._x,_y-point._y);
	}	
	Point2D<Type> operator*( const Point2D<Type> & point ) const
	{
		return Point2D<Type>(_x*point._x,_y*point._y);
	}	
	Point2D<Type> operator/( const Point2D<Type> & point ) const
	{
		return Point2D<Type>(_x/point._x,_y/point._y);
	}	
	
	Type	dot( const Point2D<Type> & point ) const
	{
		return _x*point._x + _y*point._y;
	}
	
	//! Euclidean Distance between points.
	Type distance( const Point2D<Type> & point) const
	{
		//Type valueX = abs(_x-point._x);
		Type valueX = (_x-point._x);
		//Type valueY = abs(_y-point._y);
		Type valueY = (_y-point._y);
		return sqrt(valueX*valueX + valueY*valueY);
	}
};

} // namespace Engine

#endif // __Point2D_hxx__

