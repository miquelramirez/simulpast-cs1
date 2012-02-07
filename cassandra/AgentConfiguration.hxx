
#ifndef __AgentConfiguration_hxx__
#define __AgentConfiguration_hxx__

#include <QIcon>
#include <QColor>
#include "Point3D.hxx"

namespace GUI
{
class Model3D;

class AgentConfiguration
{
	QColor _color;
	QIcon * _icon;
	bool _useIcon;
	std::string _fileName2D;
	float _size;

	Engine::Point3D<float> _size3D;
	std::string _fileName3D;
	Model3D * _model;
		
public:
	AgentConfiguration();
	AgentConfiguration( const AgentConfiguration & prototype );
	virtual ~AgentConfiguration();

	void setColor( const QColor & color );
	//void setIcon( const std::string & fileName );
	void setUseIcon( const bool & useIcon );
	void setSize( const float & size );
	void setSize3D( const Engine::Point3D<float> & size3D );
	void setFileName2D( const std::string & fileName2D);
	void setFileName3D( const std::string & fileName3D);

	const bool & useIcon() const;
	const QColor & getColor() const;
	const QIcon & getIcon() const;
	const float & getSize() const;
	const Engine::Point3D<float> & getSize3D() const;
	const std::string & getFileName2D() const;
	const std::string & getFileName3D() const;
	const Model3D & getModel() const;
};

} // namespace GUI

#endif // __AgentConfiguration_hxx__

