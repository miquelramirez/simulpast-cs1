
#ifndef __AgentTraitSelection_hxx__
#define __AgentTraitSelection_hxx__

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

class AgentTraitSelection : public QListWidget
{
	Q_OBJECT

	Engine::SimulationRecord * _simulationRecord;
	std::list<std::string> _states;
	std::string _selectedType;
public:

	AgentTraitSelection(QWidget * parent = 0);
	virtual ~AgentTraitSelection();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );

public slots:
	void typeSelected( QListWidgetItem * item );
}; 

} // namespace GUI

#endif // __AgentTraitSelection_hxx__

