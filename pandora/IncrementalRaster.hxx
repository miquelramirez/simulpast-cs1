#ifndef __IncrementalRaster_hxx__
#define __IncrementalRaster_hxx__

#include "Raster.hxx"
#include <map>

namespace Engine
{

class IncrementalRaster : public Raster
{
	typedef std::map< Point2D<int>, int >	ChangeTable;	

public:

	IncrementalRaster( const Raster& baseRaster );
	IncrementalRaster( const IncrementalRaster& other );

	virtual ~IncrementalRaster();

	virtual void		resize(  const Point2D<int> & size );

	virtual void 		setValue( Point2D<int> pos, int value );
	virtual const int& 	getValue( Point2D<int> pos );

	int			getCurrentMinValue() const { return _currentMinValue; }
	int			getCurrentMaxValue() const { return _currentMaxValue; }

	//virtual	void		updateCurrentMinMaxValues();

	typedef ChangeTable::const_iterator	ChangeIterator;

	ChangeIterator		firstChange() const { return _changes.begin(); }
	ChangeIterator		endOfChanges() const { return _changes.end(); }	

	bool			operator==( const IncrementalRaster& other ) const;

	bool			operator!=( const IncrementalRaster& other ) const
	{
		return !(this->operator==( other));
	}

	bool			operator<( const IncrementalRaster& other ) const
	{
		return _changes.size() < other._changes.size();
	}

private:

	ChangeTable		_changes;
	const Raster*		_baseRaster;
	int			_currentMinValue;
	int			_currentMaxValue;
};

}

#endif // IncrementalRaster.hxx
