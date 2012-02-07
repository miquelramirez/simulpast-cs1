
#ifndef __RasterConfigurator_hxx__
#define __RasterConfigurator_hxx__

#include <QDialog>

#include "ui_RasterConfigurator.h"

#include "RasterConfiguration.hxx"

namespace GUI
{

class RasterConfigurator : public QDialog
{
	Q_OBJECT

	std::string _type;
	Ui::RasterConfigurator _rasterConfig;

	RasterConfiguration _configuration;

public:
	RasterConfigurator(QWidget * parent, const std::string & type );
	virtual ~RasterConfigurator();

private slots:
	void transparentToggled( bool checked );
	void accept();
	void splitColorInterval();
	void removeColorInterval();

signals:
	void rasterConfigured( const std::string &, const RasterConfiguration & );
};

} // namespace GUI

#endif // __RasterConfigurator_hxx__

