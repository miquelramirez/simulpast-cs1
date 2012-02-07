
#ifndef __AgentTypeSelection_hxx__
#define __AgentTypeSelection_hxx__

//#include <QWidget>
#include <list>
#include <string>

#include <QListWidget>

class QListWidget;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{

class AgentTypeSelection : public QListWidget
{
	Q_OBJECT

	Engine::SimulationRecord * _simulationRecord;
	std::list<std::string> _types;
public:

	AgentTypeSelection(QWidget * parent = 0);
	virtual ~AgentTypeSelection();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
}; 

} // namespace GUI

#endif // __AgentTypeSelection_hxx__

