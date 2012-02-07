
#include "GenericStatistics.hxx"
#include "MeanDataPlot.hxx"
#include "SumDataPlot.hxx"
#include "StepDataPlot.hxx"

#include <QListWidgetItem>
#include <QHBoxLayout>

#include <iostream>

namespace GUI
{

GenericStatistics::GenericStatistics( QWidget * parent ) : QWidget(parent), _meanDataPlot(0), _sumDataPlot(0), _stepDataPlot(0)
{
	QHBoxLayout * layout= new QHBoxLayout;

	_stepDataPlot = new StepDataPlot(this);
	_stepDataPlot->setTitle("Samples");
	layout->addWidget(_stepDataPlot);

	_meanDataPlot = new MeanDataPlot(this);
	_meanDataPlot->setTitle("Mean");
	layout->addWidget(_meanDataPlot);

	_sumDataPlot = new SumDataPlot(this);
	_sumDataPlot->setTitle("Sum");
	layout->addWidget(_sumDataPlot);
	
	setLayout(layout);
}

GenericStatistics::~GenericStatistics()
{
}

void GenericStatistics::setSimulationRecord( Engine::SimulationRecord * simulationRecord )
{
	_meanDataPlot->setSimulationRecord(simulationRecord);
	_sumDataPlot->setSimulationRecord(simulationRecord);
	_stepDataPlot->setSimulationRecord(simulationRecord);
}

void GenericStatistics::calculateStatistics( const std::string & type, const std::string & state )
{
	std::cout << "stats for type: " << type << " and state: " << state << std::endl;
	_meanDataPlot->stateSelected(type, state);
	_sumDataPlot->stateSelected(type, state);
	_stepDataPlot->stateSelected(type, state);
}

void GenericStatistics::viewedStepChangedSlot( int newViewedStep )
{
	_stepDataPlot->stepChanged(newViewedStep);
}

} // namespace GUI

