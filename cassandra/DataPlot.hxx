
#ifndef __DataPlot_hxx__
#define __DataPlot_hxx__

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
// this basic plotter shows a given value for the entire simulation (x = step)
class DataPlot : public QwtPlot
{
	Q_OBJECT
	QwtPlotCurve * _valuePlot;
	QwtPlotZoomer * _zoomer;

	void updateMinMaxValue();
protected:
	Engine::SimulationRecord * _simulationRecord;
//	QwtArray<double> _steps;
//	QwtArray<double> _values;
	QVector<double> _steps;
	QVector<double> _values;

	double _minValue;
	double _maxValue;
public:
	DataPlot(QWidget * parent = 0);
	virtual ~DataPlot();
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
	virtual void calculateValues( const std::string & type, const std::string & state ) = 0;
	void stateSelected( const std::string & type, const std::string & state );

protected slots:
	void mouseMoved( const QPoint & point );
};

} // namespace GUI

#endif // __DataPlot_hxx__
