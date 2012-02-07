
#include "Configurator3D.hxx"
#include <iostream>
#include "ProjectConfiguration.hxx"

namespace GUI
{

Configurator3D::Configurator3D(QWidget * parent ) : QDialog(parent), _configuration(ProjectConfiguration::instance()->getConfig3D())
{
	setModal(true);
	_config3D.setupUi(this);
	
	_config3D.xSize->setValue(_configuration.getSize3D()._x);
	_config3D.ySize->setValue(_configuration.getSize3D()._y);
	_config3D.zSize->setValue(_configuration.getSize3D()._z);

	for(std::list<std::string>::const_iterator it=_configuration.beginRasters(); it!=_configuration.endRasters(); it++)
	{
		_config3D.DEMChooser->addItem(QString((*it).c_str()));
		_config3D.colorChooser->addItem(QString((*it).c_str()));
	}
	for(int i=0; i<_config3D.DEMChooser->count(); i++)
	{
		std::string name = _config3D.DEMChooser->itemText(i).toStdString();
		if(name.compare(_configuration.getDEMRaster())==0)
		{
			_config3D.DEMChooser->setCurrentIndex(i);
		}
		if(name.compare(_configuration.getColorRaster())==0)
		{
			_config3D.colorChooser->setCurrentIndex(i);
		}
	}
	show();
}

Configurator3D::~Configurator3D()
{
}

void Configurator3D::accept()
{
	std::cout << "config accepted" << std::endl;
	Engine::Point3D<float> size(_config3D.xSize->value(), _config3D.ySize->value(), _config3D.zSize->value());
	_configuration.setSize3D(size);
	_configuration.setRasters(_config3D.DEMChooser->currentText().toStdString(), _config3D.colorChooser->currentText().toStdString());
	emit configured3D(_configuration);
	close();
}

} // namespace GUI

