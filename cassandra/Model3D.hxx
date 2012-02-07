
#ifndef __Model3D_hxx__
#define __Model3D_hxx__

#include <string>
#include <map>
#include <list>

#include "Point3D.hxx"

namespace GUI
{
class Object3D;
class Material;

class Model3D
{
public:
	typedef std::map<std::string, Material * > MaterialsMap;
private:
	typedef std::list<Object3D *> ObjectsList;

	ObjectsList _objects;
	MaterialsMap _materials;

	// this method normalizes all the object of the model, in order to have a Model3D of size 1
	void normalizeModel();

	Engine::Point3D<float> _modelScale;
	void registerTextures();
public:
	Model3D();
	virtual ~Model3D();

	void paint() const;

	// this method registers a new object inside Model, and returns pointer (to fill it)
	Object3D * createNewObject( const std::string & name );

	// register Material into MaterialsMap _materials. 
	// As we don't know the name of the material when we create the material, it can't be created and registered at the same time
	void registerMaterial( Material * material, const std::string & name );

	// this method is called after loading to execute initialization actions
	void init();
	void setModelScale( const Engine::Point3D<float> & modelScale );
};

} // namespace GUI

#endif // __Model3D_hxx__

