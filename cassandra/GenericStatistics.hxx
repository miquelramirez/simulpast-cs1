
#ifndef __GenericStatics_hxx__
#define __GenericStatics_hxx__

#include <QWidget>

class QListWidgetItem;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{
class MeanDataPlot;
class SumDataPlot;
class StepDataPlot;

class GenericStatistics : public QWidget
{
	Q_OBJECT

	MeanDataPlot * _meanDataPlot;
	SumDataPlot * _sumDataPlot;
	StepDataPlot * _stepDataPlot;
public:
	GenericStatistics(QWidget * parent);
	virtual ~GenericStatistics();
	
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
public slots:
	void calculateStatistics( const std::string & type, const std::string & state );
	void viewedStepChangedSlot( int newViewedStep );
};

} // namespace GUI

#endif // __GenericStatics_hxx__

