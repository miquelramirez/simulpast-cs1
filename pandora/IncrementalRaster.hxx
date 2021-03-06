
/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
#ifndef __IncrementalRaster_hxx__
#define __IncrementalRaster_hxx__

#include <Raster.hxx>
#include <map>

namespace Engine
{

class IncrementalRaster : public Raster
{
	typedef std::map< Point2D<int>, int >	ChangeTable;	

public:
	IncrementalRaster();
	IncrementalRaster( const Raster& baseRaster );
	IncrementalRaster( const IncrementalRaster& other );

	virtual ~IncrementalRaster();

	virtual void		resize(  const Point2D<int> & size );

	virtual void 		setValue( Point2D<int> pos, int value );
	virtual const int& 	getValue( Point2D<int> pos ) const;

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
