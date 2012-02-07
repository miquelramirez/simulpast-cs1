
#ifndef __AgentConfigurator_hxx__
#define __AgentConfigurator_hxx__

#include <QDialog>
#include <QString>
#include <QColor>
#include <QListWidgetItem>
#include "AgentConfiguration.hxx"

#include "ui_AgentConfigurator.h"

class QCloseEvent;
class QIcon;

namespace GUI
{

class AgentConfigurator : public QDialog
{
	Q_OBJECT

//	std::string _selectedShape;
	std::string _type;
	Ui::AgentConfigurator _agentConfig;

	AgentConfiguration _configuration;
public:
	AgentConfigurator(QWidget * parent, const std::string & type );
	virtual ~AgentConfigurator();
	
private slots:
	/*
	void rectangleClicked();
	void circleClicked();
	void squareClicked();
	void triangleClicked();
	void starClicked();
*/
	void selectColor();
	void selectIcon();
	void selectModel3D();

	void colorSelected( const QColor & selectedColor );

	void accept(); 
signals:
	void agentConfigured( const std::string &, const AgentConfiguration & );
};

} // namespace GUI

#endif // __AgentConfigurator_hxx__

