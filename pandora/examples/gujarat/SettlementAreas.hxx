#ifndef __SettlementAreas_hxx__
#define __SettlementAreas_hxx__

#include "Rectangle.hxx"
#include "Point2D.hxx"
#include <vector>



namespace Gujarat
{

	class GujaratWorld;
		
	class SettlementAreas
	{

		std::vector< Engine::Rectangle<int> > 	_areas;
		std::vector<int> 			_scoreAreas;

	protected: 
		void updateArea(Engine::Point2D<int> & newPoint, Engine::Rectangle<int> & r);
		void setNewArea(Engine::Point2D<int> loc,GujaratWorld &w,std::vector<bool> & duneInArea);

	public:

		SettlementAreas();
		virtual ~SettlementAreas();
  
		void generateAreas(GujaratWorld &w);
		const std::vector< Engine::Rectangle<int> >& getAreas() const { return _areas; } 
		const Engine::Rectangle<int> & getAreaById(int id) const { return _areas[id]; }
		int getScoreByAreaId(int id) const { return _scoreAreas[id]; } 
 
		int ComputeAreaScore(const Engine::Rectangle<int> & newArea, GujaratWorld &w);
  
		void intersectionFilter(Engine::Rectangle<int> & r, std::vector<int> & candidates) const;
		//!! Returns a list of Area/Rectngle identifiers. The corresponding Areas have non empty
		//!! intersection with the Rectangle 'r'.
		
		friend std::ostream & operator<<( std::ostream & stream, SettlementAreas & sa )
		{
			std::vector< Engine::Rectangle<int> >:: iterator it = sa._areas.begin();
			while( it != sa._areas.end() )
			{	  
				stream << "area:[" << *it << "]";
				it++;
			}
			return stream;	  
		}
  
	};
	
	class compareSettlementAreas
	{	
		const SettlementAreas * _settlementAreas;	
	public:
	
		compareSettlementAreas( const SettlementAreas* areas )
		{
			_settlementAreas = areas;
		}
	
		bool operator()(const int& idArea1, const int& idArea2) const
		{
			return 	_settlementAreas->getScoreByAreaId(idArea1) 
					< 
					_settlementAreas->getScoreByAreaId(idArea2);
			//return idArea1 < idArea2;
		}
		
		//static setWorld(Gujarat::GujaratWorld *w) { _world = w; }
	};	
	
	
	
}//namespace

#endif
