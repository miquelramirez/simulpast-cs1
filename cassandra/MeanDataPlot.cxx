
#include "MeanDataPlot.hxx"
#include "SimulationRecord.hxx"

#include <QWidget>
#include <iostream>

namespace GUI
{

MeanDataPlot::MeanDataPlot( QWidget * parent ) : DataPlot(parent)
{
}

MeanDataPlot::~MeanDataPlot()
{
}

void MeanDataPlot::calculateValues( const std::string & type, const std::string & state )
{
	for(int i=0; i<_values.size(); i++)
	{
		_values[i] = _simulationRecord->getMean(type, state, i);
	}
}

} // namespace GUI

