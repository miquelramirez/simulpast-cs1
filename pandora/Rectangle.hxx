
#ifndef __Rectangle_hxx__
#define __Rectangle_hxx__

#include <sstream>
#include <cmath>

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

