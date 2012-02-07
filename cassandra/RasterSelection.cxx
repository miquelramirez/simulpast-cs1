
#include "RasterSelection.hxx"
#include "SimulationRecord.hxx"

#include <algorithm>

#include <iostream>

#include <QDropEvent>

namespace GUI
{

RasterSelection::RasterSelection(QWidget * parent ) : QListWidget(parent), _simulationRecord(0)
{
	setDragDropMode(QAbstractItemView::InternalMove);
	setMouseTracking(true);
}

RasterSelection::~RasterSelection()
{
}

void RasterSelection::setSimulationRecord( Engine::SimulationRecord * simulationRecord )
{
	_simulationRecord = simulationRecord;
	clear();
	if(!simulationRecord)
	{
		update();
		return;
	}
	for(Engine::SimulationRecord::RasterMap::iterator it=_simulationRecord->beginRasters(); it!=_simulationRecord->endRasters(); it++)
	{
		addItem(it->first.c_str());
	}
	for(Engine::SimulationRecord::StaticRasterMap::iterator it=_simulationRecord->beginStaticRasters(); it!=_simulationRecord->endStaticRasters(); it++)
	{
		addItem(it->first.c_str());
	}
	setCurrentRow(0);
	update();
	updateRasters();
}

void RasterSelection::updateRasters()
{
	std::list<std::string> items;
	for(int i=0; i<count(); i++)
	{
		std::string newItem(item(i)->text().toStdString());
		items.push_back(newItem);
	}
	emit rastersRearranged(items);
}

void RasterSelection::dropEvent( QDropEvent * event )
{
	QListWidget::dropEvent(event);
	updateRasters();
}


} // namespace GUI

