
#include "SumDataPlot.hxx"
#include "SimulationRecord.hxx"

#include <QWidget>

namespace GUI
{

SumDataPlot::SumDataPlot( QWidget * parent ) : DataPlot(parent)
{
}

SumDataPlot::~SumDataPlot()
{
}

void SumDataPlot::calculateValues( const std::string & type, const std::string & state )
{
	for(int i=0; i<_values.size(); i++)
	{
		_values[i] = _simulationRecord->getSum(type, state, i);
	}
}

} // namespace GUI

