
#ifndef __ColorSelector_hxx__
#define __ColorSelector_hxx__

#include "Raster.hxx"
#include <QColor>
#include "Point3D.hxx"

namespace GUI
{

// ColorSelector follows the Strategy design pattern to allow multiple choices of the way rasters are painted in OpenGL

class ColorSelector
{
public:
	ColorSelector()
	{
	}

	virtual ~ColorSelector()
	{
	}

	virtual const QColor & getColor( const int & value ) const = 0;
	virtual unsigned int getNumIntervals() const = 0;
	virtual const int & getMinValue() const = 0;
	virtual const int & getMaxValue() const = 0;
	virtual const int & getBreak( const int & index ) const = 0;
	virtual void addBreak( const int & value ) = 0;

	virtual ColorSelector * copy() const = 0;
	virtual void setColor( const QColor & color, const int & index ) = 0;
};

} // namespace GUI

#endif // __ColorSelector_hxx__

