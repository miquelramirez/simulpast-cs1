
#ifndef __Configuration3D_hxx__
#define __Configuration3D_hxx__

#include "Point3D.hxx"
#include <string>
#include <list>

namespace GUI
{

class Configuration3D
{
	Engine::Point3D<float> _size;

	std::list<std::string> _rasters;
	std::string _dem;	
	std::string _color;

public:
	Configuration3D();
	virtual ~Configuration3D();
	void setRastersList( const std::list<std::string> & rasters );

	void setSize3D( const Engine::Point3D<float> & size );
	const Engine::Point3D<float> & getSize3D() const;
	std::list<std::string>::const_iterator beginRasters() const;
	std::list<std::string>::const_iterator endRasters() const;

	void setRasters( std::string dem, std::string color );
	const std::string getDEMRaster() const;
	const std::string getColorRaster() const;
};

} // namespace GUI

#endif // __Configuration3D_hxx__

