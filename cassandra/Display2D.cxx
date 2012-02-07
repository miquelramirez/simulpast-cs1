
#include "Display2D.hxx"
#include "Raster.hxx"
#include "SimulationRecord.hxx"
#include "AgentRecord.hxx"
#include "Exceptions.hxx"
#include <QPainter>
#include <QToolTip>
#include <QListWidgetItem>
#include <QPixmap>
#include <sstream>
#include <iostream>
#include "AgentConfiguration.hxx"
#include "RasterConfiguration.hxx"
#include "ProjectConfiguration.hxx"
#include "ColorSelector.hxx"
#include <algorithm>

namespace GUI
{

Display2D::Display2D( QWidget * parent) : QWidget(parent), _simulationRecord(0), _viewedStep(0), _zoom(1), _showAgents(true), _offset(0,0), _clickedPos(0,0), _type("unknown"), _state("unknown")
{
	setMouseTracking(true);
	setAutoFillBackground(true);
	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::lightGray);
	setPalette(p);
}

Display2D::~Display2D()
{
}

void Display2D::setSimulationRecord( Engine::SimulationRecord * simulationRecord )
{
	_simulationRecord = simulationRecord;
	_viewedStep = 0;
	_zoom = 1;
	update();
}

void Display2D::paintEvent(QPaintEvent *event)
{
	if(!_simulationRecord || _orderedRasters.empty())
	{
		return;
	}
	QPixmap imageToDraw(_simulationRecord->getSize()*_zoom, _simulationRecord->getSize()*_zoom);
	QPainter painter(&imageToDraw);
	QPen pen;
	pen.setWidth(_zoom);

	imageToDraw.fill(QColor(255,0,0));

	Engine::StaticRaster & rasterTmp(_simulationRecord->getRasterTmp(*(_orderedRasters.begin()), _viewedStep));
	Engine::Point2D<int> size = rasterTmp.getSize();
	for(int i=0; i<size._x; i++)
	{
		for(int j=0; j<size._y; j++)
		{
			std::list<std::string>::iterator it =_orderedRasters.end();
			while(it!=_orderedRasters.begin())
			{
				it--;
				RasterConfiguration * rasterConfig = ProjectConfiguration::instance()->getRasterConfig(*it);
				Engine::StaticRaster & raster(_simulationRecord->getRasterTmp(*it, _viewedStep));
				int value = raster.getValue(Engine::Point2D<int>(i,j));
				if(rasterConfig->isTransparentEnabled() && value==rasterConfig->getTransparentValue())
				{
					continue;
				}
				ColorSelector & colorSelector = rasterConfig->getColorRamp();
				pen.setColor(colorSelector.getColor(value));
				/*
				//pen.setWidth(_zoom);
				if(maxValue>0)
				{
					int colorValue = 255*value/maxValue;
					pen.setColor(QColor(0,colorValue, 0));
				}
				else
				{
					pen.setBrush(QColor(200,200,200));
				}
				*/
				painter.setPen(pen);
				// horizontal
				//painter.drawLine(i*_zoom, j*_zoom, (i+1)*_zoom, j*_zoom);
				//painter.drawLine(i*_zoom, (j+1)*_zoom, (i+1)*_zoom, (j+1)*_zoom);
				// vertical
				//painter.drawLine(i*_zoom, j*_zoom, i*_zoom, (j+1)*_zoom);
				//painter.drawLine((i+1)*_zoom, j*_zoom, (i+1)*_zoom, (j+1)*_zoom);
				painter.drawPoint(i*_zoom+_zoom/2,j*_zoom+_zoom/2);
				//QPolygon foo(QRect(i*_zoom, j*_zoom, _zoom, _zoom));
				//painter.drawPolygon(foo);
				//painter.drawRect(i*_zoom,j*_zoom, _zoom, _zoom);
				break;
			}
		}
	}
	
	if(!_showAgents)
	{
		QPainter screenPainter(this);	
		screenPainter.save();
		screenPainter.drawPixmap(_offset, imageToDraw); //.scaled(_simulationRecord->getSize()*_zoom, _simulationRecord->getSize()*_zoom));
		screenPainter.restore();
		return;
	}
	
	QPen agentsPen;
	agentsPen.setWidth(_zoom);
	
	for(Engine::SimulationRecord::AgentTypesMap::iterator itType = _simulationRecord->beginTypes(); itType!=_simulationRecord->endTypes(); itType++)
	{
		AgentConfiguration * agentConfig = ProjectConfiguration::instance()->getAgentConfig(itType->first);
		for(Engine::SimulationRecord::AgentRecordsMap::iterator it= _simulationRecord->beginAgents(itType); it!=_simulationRecord->endAgents(itType); it++)
		{
			if(agentConfig->useIcon())
			{	
				Engine::AgentRecord * agent = it->second;
				bool exists = agent->getState(_viewedStep/_simulationRecord->getResolution(), "exists");
				if(exists)
				{
					int x = agent->getState(_viewedStep/_simulationRecord->getResolution(), "x");
					int y = agent->getState(_viewedStep/_simulationRecord->getResolution(), "y");
					int size = (float)_zoom*agentConfig->getSize();
					int xPos = x*_zoom - size/2;						
					int yPos = y*_zoom - size/2;
					agentConfig->getIcon().paint(&painter, QRect(xPos, yPos, size, size));
				}
			}
			else
			{
				QColor colorToUse(agentConfig->getColor());
				Engine::AgentRecord * agent = it->second;
				bool exists = agent->getState(_viewedStep/_simulationRecord->getResolution(), "exists");
				if(exists)
				{
					int x = agent->getState(_viewedStep/_simulationRecord->getResolution(), "x");
					int y = agent->getState(_viewedStep/_simulationRecord->getResolution(), "y");
					if(_state=="unknown")
					{
						agentsPen.setBrush(colorToUse);
						painter.setPen(agentsPen);
					}
					else
					{
						// we put this call between try/catch in order to avoid crashes about painting state in agents that don't have it
						int value = 0;
						try
						{
							value = agent->getState(_viewedStep/_simulationRecord->getResolution(), _state);
							int max = _simulationRecord->getMaxValueForState(_state);
							int min = _simulationRecord->getMinValueForState(_state);
							int diff = std::max(1,max - min);
							value = (value-min)*255/diff;
						}
						catch( Engine::Exception & exceptionThrown )
						{
						}
						agentsPen.setBrush(QColor(255,255-value,255-value));
						painter.setPen(agentsPen);
					}

					painter.drawPoint(x*_zoom+_zoom/2, y*_zoom+_zoom/2);
					/*
					std::string shape("circle");
					AgentsShapeMap::iterator itShape= _agentsShape.find(itType->first);
					if(itShape!=_agentsShape.end())
					{
						shape = itShape->second;
					}

					if(shape.compare("rectangle")==0)
					{
						painter.drawRect(x,y,2,1);
					}
					else if(shape.compare("circle")==0)
					{
						painter.drawEllipse(x,y,1,1);
					}
					else if(shape.compare("square")==0)
					{
						painter.drawRect(x,y,1,1);
					}
					else if(shape.compare("triangle")==0)
					{
						QPointF points[3];
						points[0] = QPointF(x-1,y-1);
						points[1] = QPointF(x+1,y-1);
						points[2] = QPointF(x,y+1);

						painter.drawPolygon(points, 3);
					}
					else if(shape.compare("star")==0)
					{
						QPointF points[5];
						points[0] = QPointF(x+1, y+0.5);
						for (int i = 1; i < 5; ++i)
						{
							points[i] = QPointF(x + 0.5 + 0.5 * cos(0.8 * i * 3.14), y+ 0.5 + 0.5 * sin(0.8 * i * 3.14));
						}
						painter.drawPolygon(points, 5);
					}
					*/
				}
			}
		}
	}
	QPainter screenPainter(this);	
	screenPainter.save();
	screenPainter.drawPixmap(_offset, imageToDraw); //.scaled(_simulationRecord->getSize()*_zoom, _simulationRecord->getSize()*_zoom));
	screenPainter.restore();
}

void Display2D::typeSelected( QListWidgetItem * item )
{
	_type = item->text().toStdString();
	_state = "unknown";
	// actualitzar estats
	update();
}
void Display2D::stateSelected( QListWidgetItem * item )
{
	_state = item->text().toStdString();
	if(_type!="unknown" && _state!="unknown")
	{
		emit calculateStatistics(_type,_state);
	}
	update();
}

void Display2D::rastersRearranged( std::list<std::string> items )
{
	_orderedRasters.clear();
	_orderedRasters.resize(items.size());
	std::reverse_copy(items.begin(), items.end(), _orderedRasters.begin());
	update();
}

QSize Display2D::minimumSizeHint() const
{
	return QSize(200, 200);
}

QSize Display2D::sizeHint() const
{
	return QSize(800, 800);
}

void Display2D::viewedStepChangedSlot( int newViewedStep )
{
	_viewedStep = newViewedStep;
	update();
}

void Display2D::zoomIn()
{
	zoom(1);
}

void Display2D::zoomOut()
{
	zoom(-1);
}

void Display2D::zoom( int value )
{
	if(!_simulationRecord)
	{
		return;
	}
	_zoom += value;
	if(_zoom<1)
	{
		_zoom = 1;
	}
	else if(_zoom>30)
	{
		_zoom = 30;
	}
	update();
}

void Display2D::wheelEvent( QWheelEvent * event )
{
	float numDegrees = event->delta();
	numDegrees /= 120.0f;
	zoom(numDegrees);
}

void Display2D::mousePressEvent (QMouseEvent * event)
{
	if(event->button()==Qt::RightButton)
	{
		_clickedPos = event->globalPos() - _offset;
	}
}

void Display2D::mouseMoveEvent (QMouseEvent * event)
{
	if(!_simulationRecord)
	{
		return;
	}
	if(event->buttons() & Qt::RightButton)
	{
		_offset = event->globalPos() - _clickedPos;
		update();
	}
}

std::string Display2D::getRasterToolTip( const Engine::Point2D<int> & position )
{	
	std::stringstream toolTipString;
	std::list<std::string>::iterator it =_orderedRasters.end();
	while(it!=_orderedRasters.begin())
	{
		it--;
		Engine::StaticRaster & raster = _simulationRecord->getRasterTmp(*it, _viewedStep);
		toolTipString << *it << " in " << position << " : " << raster.getValue(position) << std::endl;
	}
	return toolTipString.str();
}

std::string Display2D::getAgentToolTip( const Engine::Point2D<int> & position )
{
	std::stringstream toolTipString;
	Engine::AgentRecord * agentRecord = _simulationRecord->getAgentAtPosition(_viewedStep/_simulationRecord->getResolution(), position);
	if(agentRecord)
	{
		toolTipString << std::endl << agentRecord->getCompleteState(_viewedStep/_simulationRecord->getResolution());
	}
	return toolTipString.str();
}


bool Display2D::event(QEvent *event)
{  
	if(_simulationRecord && event->type()==QEvent::ToolTip) 
	{
		if(_orderedRasters.empty())
		{
			return QWidget::event(event);
		}

		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		Engine::Point2D<int> position(helpEvent->pos().x()-_offset.x(), helpEvent->pos().y()-_offset.y());
		// TODO program /= i *= in Engine::Point2D
		position._x /= _zoom;
		position._y /= _zoom;

		Engine::StaticRaster & raster = _simulationRecord->getRasterTmp(*(_orderedRasters.begin()), _viewedStep);
		if(position._x<0 || position._y<0 || position._x>=raster.getSize()._x || position._y>=raster.getSize()._y)
		{
			return QWidget::event(event);
		}

		std::string finalToolTip("");
		finalToolTip += getRasterToolTip(position);
		finalToolTip += getAgentToolTip(position);		
		
		QToolTip::showText(helpEvent->globalPos(), finalToolTip.c_str());
     }
     return QWidget::event(event);
}

void Display2D::showAgents()
{
	_showAgents = !_showAgents;
	update();
}

} // namespace GUI

