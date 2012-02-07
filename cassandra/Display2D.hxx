
#ifndef __Display2D_hxx__
#define __Display2D_hxx__

#include "Point2D.hxx"

#include <QWidget>
#include <QWheelEvent>
#include <vector>
#include <string>

class QListWidgetItem;

namespace Engine
{
	class SimulationRecord;
	class Raster;
}

namespace GUI
{
class AgentConfiguration;
class RasterConfiguration;

class Display2D : public QWidget
{
	Q_OBJECT
	
	typedef std::map<std::string, AgentConfiguration *> AgentsConfigurationMap;
	typedef std::map<std::string, RasterConfiguration *> RastersConfigurationMap;

	Engine::SimulationRecord * _simulationRecord;
	
	int _viewedStep;
	
	int _zoom;
	bool _showAgents;

	void zoom( int value );

	QPoint _offset;
	QPoint _clickedPos;

	// state being selected
	std::string _type;
	std::string _state;
	// order of rasters
	std::list<std::string> _orderedRasters;

	std::string getRasterToolTip( const Engine::Point2D<int> & position );
	std::string getAgentToolTip( const Engine::Point2D<int> & position );
public:
	Display2D(QWidget * parent);
	virtual ~Display2D();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:  
	bool event(QEvent *event);
	void paintEvent(QPaintEvent * event);
	void wheelEvent(QWheelEvent * event);
	void mousePressEvent(QMouseEvent * event);	
	void mouseMoveEvent(QMouseEvent * event);	
public slots:
	void viewedStepChangedSlot( int newViewedStep );
	void zoomIn();
	void zoomOut();
	void showAgents();
	void typeSelected( QListWidgetItem * item);
	void stateSelected( QListWidgetItem * item);
	void rastersRearranged( std::list<std::string> items );
signals:
	void calculateStatistics( const std::string &, const std::string & );
};

} // namespace GUI

#endif // __Display2D_hxx__

