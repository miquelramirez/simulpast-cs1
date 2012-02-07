
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

