/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es
 *
 * This file is part of Cassandra.
 * Cassandra is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Cassandra is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "LoadingProgressBar.hxx"
#include "ProjectConfiguration.hxx"
#include "SimulationRecord.hxx"
#include <iostream>

namespace GUI
{

LoadingProgressBar::LoadingProgressBar()
{
	_loadingProgressBar.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
	_loadingProgressBar.state->setText("no loading");

}

LoadingProgressBar::~LoadingProgressBar()
{
}

void LoadingProgressBar::initLoading()
{
	_loadingProgressBar.bar->setRange(0,100);
	_loadingProgressBar.bar->setValue(0);
	_loadingProgressBar.state->setText("loading...");
	show();
}

void LoadingProgressBar::updateProgress()
{
	if(!ProjectConfiguration::instance()->getSimulationRecord())
	{
		hide();
		return;
	}
	_loadingProgressBar.bar->setValue(ProjectConfiguration::instance()->getSimulationRecord()->getLoadingPercentageDone());
	_loadingProgressBar.state->setText(ProjectConfiguration::instance()->getSimulationRecord()->getLoadingState().c_str());
	update();
}

} // namespace GUI

