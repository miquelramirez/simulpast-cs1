
#include <queue>
#include "SettlementAreas.hxx"

namespace Gujarat
{

	SettlementAreas::SettlementAreas()
	{

	}

SettlementAreas::~SettlementAreas()
{
}


	void SettlementAreas::updateArea(Engine::Point2D<int> & newPoint, Engine::Rectangle<int> & r)
	{

		if (newPoint._x < r._origin._x)
		{
			r._origin._x = newPoint._x;
			r._size._x += r._origin._x - newPoint._x;
		}
		else if (newPoint._x > r._origin._x + r._size._x)
		{
			r._size._x = r._origin._x - newPoint._x;    
		}
  
  
		if (newPoint._y < r._origin._y)
		{
			r._origin._y = newPoint._y;
			r._size._y += r._origin._y - newPoint._y;
		}   
		else if (newPoint._y > r._origin._y + r._size._y)
		{
			r._size._y = r._origin._y - newPoint._y;    
		}
    
	}

	void SettlementAreas::setNewArea(Engine::Point2D<int> loc,GujaratWorld &w,std::vector<bool> & duneInArea)
	{
		int Xsize = w.getBoundaries()._size._x;

		Engine::Rectangle<int> newArea(loc,loc);  
		std::queue<Engine::Point2D<int> *> targets;
  
		targets.push( new Engine::Point2D<int>(loc._x, loc._y) );  

		while(!targets.empty())
		{
			Engine::Point2D<int> * currentLoc = targets.front();
			targets.pop();
			updateArea( *currentLoc, newArea );
			duneInArea[ currentLoc->_x*Xsize + currentLoc->_y ] = true;
			//currentx  = currentLoc->_x
			//currenty  = currentLoc->_y
			//change currentLoc to feed to isInside ( currentLoc ).
			// for should range from currentx and currenty
			for(int x = currentLoc->_x -1; x <= currentLoc->_x +1; x++)
				for(int y = currentLoc->_y -1; y <= currentLoc->_y +1; y++)
				{
					if ( !newArea.isInside(Engine::Point2D<int>(x,y)) && w.getValue("resourceType",loc)==DUNE )						
					{	  
						targets.push(new Engine::Point2D<int>(x,y));
					}
				}
			delete currentLoc;
		}
		_Areas.push_back(newArea);
	}

	void SettlementAreas::generateAreas(GujaratWorld &w)
	{
		int Xorigin = w.getBoundaries()._origin._x;
		int Yorigin = w.getBoundaries()._origin._y;
  
		int Xsize = w.getBoundaries()._size._x;
		int Ysize = w.getBoundaries()._size._y;
  
		std::vector<bool> duneInArea(Xsize*Ysize,false);
		Engine::Point2D<int> loc;
		for(loc._x = Xorigin; loc._x< Xorigin+Xsize; loc._x++)		
		{
			for(loc._y = Yorigin; loc._y< Yorigin+Ysize; loc._y++)		
			{
				if (w.getValue("resourceType",loc)==DUNE && !duneInArea[loc._x*Xsize + loc._y])
				{
					setNewArea(loc,w,duneInArea);
				}
			}
		}
	}

}//namespace
