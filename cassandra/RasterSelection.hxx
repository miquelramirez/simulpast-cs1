
#ifndef __RasterSelection_hxx__
#define __RasterSelection_hxx__

//#include <QWidget>
#include <list>
#include <string>

#include <QListWidget>

class QDropEvent;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{

class RasterSelection : public QListWidget
{
	Q_OBJECT

	Engine::SimulationRecord * _simulationRecord;
	std::list<std::string> _states;
	void dropEvent( QDropEvent * event);
	void updateRasters();
public:

	RasterSelection(QWidget * parent = 0);
	virtual ~RasterSelection();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );

signals:
	void rastersRearranged(std::list<std::string> items);
}; 

} // namespace GUI

#endif // __RasterSelection_hxx__

