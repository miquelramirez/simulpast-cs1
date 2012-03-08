

#ifndef __SettlementAreas_hxx__
#define __SettlementAreas_hxx__

#include "GujaratWorld.hxx"
#include "Rectangle.hxx"
#include "Point2D.hxx"
#include <vector>



namespace Gujarat
{


	class SettlementAreas
	{

		std::vector< Engine::Rectangle<int> > _Areas;


	protected: 
		void updateArea(Engine::Point2D<int> & newPoint, Engine::Rectangle<int> & r);
		void setNewArea(Engine::Point2D<int> loc,GujaratWorld &w,std::vector<bool> & duneInArea);

	public:

		SettlementAreas();
		virtual ~SettlementAreas();
  
		void generateAreas(GujaratWorld &w);
		const std::vector< Engine::Rectangle<int> >& getAreas() const { return _Areas; } 
  
		friend std::ostream & operator<<( std::ostream & stream, SettlementAreas & sa )
		{
			std::vector< Engine::Rectangle<int> >:: iterator it = sa._Areas.begin();
			while( it != sa._Areas.end() )
			{	  
				stream << "area:[" << *it << "]";
				it++;
			}
			return stream;	  
		}
  
	};

}//namespace

#endif
