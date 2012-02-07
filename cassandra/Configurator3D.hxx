
#ifndef __Configurator3D_hxx__
#define __Configurator3D_hxx__

#include <QDialog>
#include "ui_Configurator3D.h"

#include "Configuration3D.hxx"

namespace GUI
{
class Configurator3D : public QDialog
{
	Q_OBJECT
	
	Ui::Configurator3D _config3D;
	Configuration3D _configuration;

public:
	Configurator3D(QWidget * parent );
	virtual ~Configurator3D();

private slots:
	void accept();

signals:
	void configured3D( const Configuration3D & );

};

} // namespace GUI

#endif // __Configurator3D_hxx__

