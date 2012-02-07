
#ifndef __Material_hxx__
#define __Material_hxx__

#include <string>
#include <GL/gl.h>
#include <il.h>

#include "Point3D.hxx"

namespace GUI
{

class Material
{
	std::string _textureFileName;

	// texture id inside DevIL
	ILuint _imageId;
	// texture id inside openGL
	GLuint _textureId;

	Engine::Point3D<GLfloat> _ambientColor;
	Engine::Point3D<GLfloat> _diffuseColor;
	Engine::Point3D<GLfloat> _specularColor;

	void loadTextureImage();
	
public:
	Material();
	virtual ~Material();

	void registerTexture();
	void setTextureFileName( const std::string & textureFileName );
	void setAmbientColor( const Engine::Point3D<GLint> & ambientColor );
	void setDiffuseColor( const Engine::Point3D<GLint> & diffuseColor );
	void setSpecularColor( const Engine::Point3D<GLint> & specularColor);

	bool usesTexture();
	void activate();
	void deactivate();
};

} // namespace GUI

#endif // __Material_hxx__

