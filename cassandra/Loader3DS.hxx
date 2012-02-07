
#ifndef __Loader3DS_hxx__
#define __Loader3DS_hxx__

#include <string>
#include <fstream>

#include "Point3D.hxx"

namespace GUI
{

class Model3D;
class Object3D;

// this class is used to load a model generated with 3D Studio Max into Cassandra (as an agent)
// it is constructed as a singleton
class Loader3DS
{
	static Loader3DS * _instance;

	// this pointer is used to load a new model. it is not responsible of deleting it
	Model3D * _model;

	// helper to construct object
	Object3D * _object;

	// utility functions
	std::string loadName( std::ifstream & file );
	Engine::Point3D<int> loadColor( std::ifstream & file );
	void skipChunk( const int & length, std::ifstream & file );

	void loadVertexs( std::ifstream & file );
	void loadTextureMapCoordinates( std::ifstream & file );
	void loadPolygons( std::ifstream & file );

	void loadFaceMaterials(std::ifstream & file );

	void loadMaterial( std::ifstream & file, unsigned short & id, int & length );
protected:
	Loader3DS();	

public:
	static Loader3DS * instance();
	virtual ~Loader3DS();
	Model3D * loadModel( const std::string & file );
}; 

} // namespace GUI

#endif // __Loader3DS_hxx__

