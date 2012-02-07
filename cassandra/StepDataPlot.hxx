
#ifndef __StepDataPlot_hxx__
#define __StepDataPlot_hxx__

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotZoomer;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{
// TODO convert data plot children to template
// this basic plotter shows the complete set of values for the current step
class StepDataPlot : public QwtPlot
{
	Q_OBJECT
	QwtPlotCurve * _valuePlot;
	QwtPlotZoomer * _zoomer;

protected:
	Engine::SimulationRecord * _simulationRecord;
	QVector<double> _values;
	QVector<double> _numSamples;
//	QwtArray<double> _values;
//	QwtArray<double> _numSamples;

	int _minValue;
	int _maxValue;

	std::string _type;
	std::string _state;
	int _step;
	void updateData();
	
	//void fillHistogram( const std::string & type, const std::string & state, int step, QwtArray<double> & numSamples, const int & minValue );
	void fillHistogram( const std::string & type, const std::string & state, int step, QVector<double> & numSamples, const int & minValue );
	void fillStepMinMaxValues( const std::string & type, const std::string & state, int step, int & minValue, int & maxValue );
public:
	StepDataPlot(QWidget * parent = 0);
	virtual ~StepDataPlot();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
	void stateSelected( const std::string & type, const std::string & state );
	void stepChanged(int step );
};

} // namespace GUI

#endif // __StepDataPlot_hxx__

