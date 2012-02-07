
#ifndef __Display3D_hxx__
#define __Display3D_hxx__

#include <map>
#include "Material.hxx"

#include <QGLWidget>
#include "Point3D.hxx"

class QListWidgetItem;

namespace Engine
{
	class SimulationRecord;
}

namespace GUI
{
class Model3D;
class AgentConfiguration;
class RasterConfiguration;
class Configuration3D;

class Display3D : public QGLWidget
{
	Q_OBJECT
	
	typedef std::map<std::string, Model3D * > Models3DMap;
	typedef std::map<std::string, std::string > FileNamesMap;
	typedef std::map<std::string, AgentConfiguration *> AgentsConfigurationMap;
	typedef std::map<std::string, RasterConfiguration *> RastersConfigurationMap;
public:
	Display3D(QWidget *parent);
	virtual ~Display3D();
	//Funcions que definiràn el tamany del quadre on mostrarem el raster.
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setSimulationRecord( Engine::SimulationRecord * simulationRecord );
// 	
//Declarem els SLOTS públics que seràn les funcions que rebràn un 
//paràmetre que serà l'angle amb el que rotarem el nostre ràster
public slots:
	void viewedStepChangedSlot( int newViewedStep );
	void rastersRearranged( std::list<std::string> items );
	
//Declarem les variables i métodes que farem servir dins la nostra 
//clase. 
protected:
	//TODO Zoom y moviments amb el mouse
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void wheelEvent(QWheelEvent * event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseWheelEvent(QMouseEvent *event);
	void paintLandscape();
	void paintAgents();

	void registerModel( const std::string & key, AgentConfiguration & agentConfig, const RastersConfigurationMap & rastersConfig );
	void setCellColor( const QColor & color );
private:
	void normalizeAngle(int &angle);
	Engine::SimulationRecord * _simulationRecord;
	int _viewedStep;
	Models3DMap _models;
	// we need this map in order to check if models are new or already loadd (from registerModels)
	FileNamesMap _paths;
	Material _landscapeMaterial;

	float _zoom;
	QPoint _position;
	QPoint _targetPosition;
	QPoint _lastPos;
	// TODO improve mouse navigation. At the moment this attribute is not used
	QPoint _lastTargetPos;	
	QPoint _rotation;
	int _rotationZ;

	Engine::Point3D<float> _cellScale;

	// order of rasters
	std::list<std::string> _orderedRasters;

	const Configuration3D & _config3D;
};

}

#endif // __Display3D_hxx__

