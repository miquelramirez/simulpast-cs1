
#ifndef __DefaultColorSelector_hxx__
#define __DefaultColorSelector_hxx__

#include "ColorSelector.hxx"
#include "Raster.hxx"
#include "Point3D.hxx"

namespace GUI
{

// default selector, the choice is made based on a scale used in Digital Elevation Models
class DefaultColorSelector : public ColorSelector
{
	// the numbers that function as "breakers" between intervals	
	std::vector<int> _intervals;
	// colors of the ramp
	std::vector<QColor> _colors;

public:
	DefaultColorSelector( const int & minValue, const int & maxValue );
	DefaultColorSelector( const DefaultColorSelector & prototype );
	virtual ~DefaultColorSelector();

	void addBreak(  const int & value );
	const QColor & getColor( const int & value ) const;

	unsigned int getNumIntervals() const;
	const int & getMinValue() const;
	const int & getMaxValue() const;
	const int & getInterval( const int & index ) const;
	const int & getBreak( const int & index ) const;
	void setColor( const QColor & color, const int & index );
	ColorSelector * copy() const;
};

} // namespace GUI

#endif // __DefaultColorSelector_hxx__

