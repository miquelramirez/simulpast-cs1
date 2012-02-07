#include <iostream>
#include "Object3D.hxx"
#include "Exceptions.hxx"
#include "Material.hxx"

namespace GUI
{

Object3D::Object3D( const std::string & name, Model3D::MaterialsMap & materials ) : _name(name), _materials(materials)
{
}

Object3D::~Object3D()
{
}

void Object3D::paint( const Engine::Point3D<float> & modelScale ) const
{
	glPushMatrix();
//	glColor3f(1.0f, 1.0f, 1.0f);
	//std::cout << "painting object: " << _name << std::endl;

	Model3D::MaterialsMap::iterator	currentMaterialIt(_materials.end());
	Material * currentMaterial = 0;

	for(int i=0; i<_polygons.size(); i++)
	{
		Model3D::MaterialsMap::iterator materialIt = _materials.find(_polygonMaterials[i]);
		//std::cout << "selecting material: " << materialIt->first << std::endl;
		if(materialIt==_materials.end())
		{
			std::stringstream oss;
			oss << "Object3D::paint - polygon: " << i << " with unknown material: " << _polygonMaterials[i];
			throw Engine::Exception(oss.str());
			return;
		}
		if(currentMaterialIt!=materialIt)
		{
			currentMaterialIt = materialIt;
			currentMaterial = materialIt->second;

			currentMaterial->activate();
		}

		int index1 = _polygons[i]._x;
		int index2 = _polygons[i]._y;
		int index3 = _polygons[i]._z;

		glNormal3f(_normals[i]._x, _normals[i]._y, _normals[i]._z);

		glBegin(GL_TRIANGLES);
		
		if(currentMaterial->usesTexture())
		{
			glTexCoord2f(_textureMapCoordinates[index1]._x, _textureMapCoordinates[index1]._y);
		}
		glVertex3f(_vertexs[index1]._x*modelScale._x, _vertexs[index1]._y*modelScale._y, _vertexs[index1]._z*modelScale._z);

		if(currentMaterial->usesTexture())
		{
			glTexCoord2f(_textureMapCoordinates[index2]._x, _textureMapCoordinates[index2]._y);
		}
		glVertex3f(_vertexs[index2]._x*modelScale._x, _vertexs[index2]._y*modelScale._y, _vertexs[index2]._z*modelScale._z);
		
		if(currentMaterial->usesTexture())
		{
			glTexCoord2f(_textureMapCoordinates[index3]._x, _textureMapCoordinates[index3]._y);
		}
		glVertex3f(_vertexs[index3]._x*modelScale._x, _vertexs[index3]._y*modelScale._y, _vertexs[index3]._z*modelScale._z);	
		glEnd();

		//std::cout << "tex coords: " << _textureMapCoordinates[index1] << " - " << _textureMapCoordinates[index2] << " - " << _textureMapCoordinates[index3] << std::endl;
	}
	if(currentMaterial)
	{
		currentMaterial->deactivate();
	}
	glPopMatrix();
}

void Object3D::calculateNormals()
{
	_normals.resize(_polygons.size());
	for(int i=0; i<_normals.size(); i++)
	{
		Engine::Point3D<float> a = _vertexs[_polygons[i]._y];
		a = a - _vertexs[_polygons[i]._x];
		Engine::Point3D<float> b = _vertexs[_polygons[i]._z];
		b = b - _vertexs[_polygons[i]._y];
		Engine::Point3D<float> c = a.crossProduct(b);
		c = c.normalize();
		_normals[i] = c;
		//std::cout << "polygon: " << i << " - " << _polygons[i] << " has normals: " << _normals[i] << std::endl;
	}
}	

} // namespace GUI

