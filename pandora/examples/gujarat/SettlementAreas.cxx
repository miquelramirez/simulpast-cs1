
#include <queue>
#include "GujaratWorld.hxx"
#include "SettlementAreas.hxx"
#include <assert.h>
#include <math.h>

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
			r._size._x += r._origin._x - newPoint._x;
			r._origin._x = newPoint._x;
		}
		else if (newPoint._x > r._origin._x + (r._size._x -1) )
		{
			r._size._x = newPoint._x - r._origin._x + 1;    
		}

		if (newPoint._y < r._origin._y)
		{
			r._size._y += r._origin._y - newPoint._y;
			r._origin._y = newPoint._y;
		}
		else if (newPoint._y > r._origin._y + (r._size._y -1) )
		{
			r._size._y = newPoint._y - r._origin._y + 1;    
		}
	}
	
	int SettlementAreas::ComputeAreaScore(const Engine::Rectangle<int> & newArea, GujaratWorld &w)
	{
	int result = 0;
/*
	int wilds    = 0;
	float dunes  = 0;
	float cells  = 0;
*/	
	float wilds  = 0.0;
	float dunes  = 0.0;
	float cells  = 0.0;	
	
		Engine::Point2D<int> index;
		for(index._x = newArea._origin._x; index._x < newArea._origin._x + newArea._size._x ; index._x++)
		{
			for(index._y = newArea._origin._y; index._y < newArea._origin._y + newArea._size._y ; index._y++)
			{	
				cells++;
				if (w.getValue("soils",index) != DUNE)
				{
					wilds++;
				}
				if (w.getValue("soils",index) == DUNE)
				{
					dunes++;
				}
			}
		}
		dunes++;
		cells++;
		wilds++;
		result = (int)(1000.0*(   (1.0 - (dunes/cells)) + (wilds/cells) + (1.0/sqrt(sqrt(cells)))  ));
		//result = wilds;
		//std::cout << "SET SCORE:" << result << std::endl;
		
	return result;
	}
	
	void SettlementAreas::setNewArea(Engine::Point2D<int> loc,GujaratWorld &w,std::vector<bool> & duneInArea)
	{
		int Xsize = w.getBoundaries()._size._x;
		int Ysize = w.getBoundaries()._size._y;
	
		// to begin
		Engine::Rectangle<int> newArea(loc,Engine::Point2D<int>(1,1));  
		std::queue<Engine::Point2D<int> *> targets;
  
		updateArea( loc, newArea );
		duneInArea[ loc._x*Xsize + loc._y ] = true;

		targets.push( new Engine::Point2D<int>(loc._x, loc._y) );  

		while(!targets.empty())
		{
			Engine::Point2D<int> * currentLoc = targets.front();
			targets.pop();

			for(int childX = currentLoc->_x - 1; childX <= currentLoc->_x +1; childX++)
				for(int childY = currentLoc->_y - 1; childY <= currentLoc->_y +1; childY++)
				{
					if( childX < 0 || childY < 0 ) continue;
					if( childX >= Xsize || childY >= Ysize ) continue;
		
					Engine::Point2D<int> newPoint = Engine::Point2D<int>(childX,childY);
	      
					//std::cout << "new  location: childX = "<<childX <<" childY= "<< childY << " IsDune: " << (w.getValue("soils",newPoint) == DUNE) <<std::endl;											
					if ( duneInArea[ newPoint._x*Xsize + newPoint._y ] == false && w.getValue("duneMap",newPoint)==DUNE ) //!newArea.isInside( newPoint ) 						
					{	
//						assert( w.getValue("soils",*currentLoc)==DUNE );
//						assert( ! ( (childX >= newArea._origin._x) && (childX <= newArea._origin._x + newArea._size._x -1) && (childY >= newArea._origin._y) && (childY <= newArea._origin._y + newArea._size._y -1)));


						//std::cout << "\t Checking new dune location: childX = "<<childX <<" childY= "<< childY <<std::endl;						
						//std::cout<<"\t\t childX "<< childX <<" in [ " << newArea._origin._x <<",.., "<<  newArea._origin._x + newArea._size._x -1<<" ]"<<std::endl;
						//std::cout<<"\t\t childY "<< childY <<" in [ " << newArea._origin._y <<",.., "<<  newArea._origin._y + newArea._size._y -1<<" ]"<<std::endl;

						
						updateArea( newPoint, newArea );
						duneInArea[ newPoint._x*Xsize + newPoint._y ] = true;
						
						
						//					std::cout<< " Area Updated: "<< newArea << std::endl;


						targets.push( new Engine::Point2D<int>( newPoint._x, newPoint._y ) );
					}

				}
			delete currentLoc;

//			std::cout<<"targets size: "<< targets.size()<<std::endl;

			
		}
		//std::cout << loc._x << " "<< loc._y << " newArea: "<<newArea<<std::endl;
		
		_areas.push_back(newArea);
		_scoreAreas.push_back(ComputeAreaScore(newArea,w));
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
				if (w.getValue("duneMap",loc)==DUNE && !duneInArea[loc._x*Xsize + loc._y])
				{
					//std::cout <<"origin area: "<< loc << std::endl;
					setNewArea(loc,w,duneInArea);
					
				}
			}
		}
	}

	void SettlementAreas::intersectionFilter(Engine::Rectangle<int> & r, std::vector<int> & candidates) const
	{	
		// looping in search of candidates
		Engine::Rectangle<int> intersection;
		for(unsigned long i=0; i < _areas.size(); i++)
		{
			if(r.intersection(_areas[i],intersection))
			{
			//	if( insideTheCircle(intersection,a._location,a._homeRange) )
			//		insideTheCircle, an extra filter, future issue	
				candidates.push_back(i);
			}
			
		}
	}
		
}//namespace
