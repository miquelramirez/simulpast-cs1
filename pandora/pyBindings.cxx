
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include "Point2D.hxx"
#include "StaticRaster.hxx"
#include "Raster.hxx"
#include "Simulation.hxx"
#include "World.hxx"
#include "Agent.hxx"

typedef Engine::Point2D<int> Point2DInt;

class StaticRasterWrap : public Engine::StaticRaster, public boost::python::wrapper<Engine::StaticRaster>
{
public:
	void resize( const Point2DInt & size )
	{
		if (boost::python::override resize = this->get_override("resize")(size))			
		{
			resize( size );
			return;
		}
		Engine::StaticRaster::resize(size);
	}

	void default_resize( const Point2DInt & size )
	{
		Engine::StaticRaster::resize(size);
	}
	
	const int & getValue( Point2DInt position ) const
	{
		if (boost::python::override getValue = this->get_override("getValue")(position))
		{
			return getValue(position);
		}
		return Engine::StaticRaster::getValue(position);
	}

	const int & default_getValue(Point2DInt position) const
	{
		return Engine::StaticRaster::getValue(position);
	}
};

class RasterWrap : public Engine::Raster, public boost::python::wrapper<Engine::Raster>
{
public:
	void resize( const Point2DInt & size )
	{
		if (boost::python::override resize = this->get_override("resize")(size))			
		{
			resize( size );
			return;
		}
		Engine::Raster::resize(size);
	}

	void default_resize( const Point2DInt & size )
	{
		Engine::Raster::resize(size);
	}
	
	const int & getValue( Point2DInt position ) const
	{
		if (boost::python::override getValue = this->get_override("getValue")(position))
		{
			return getValue(position);
		}
		return Engine::Raster::getValue(position);
	}

	const int & default_getValue(Point2DInt position) const
	{
		return Engine::Raster::getValue(position);
	}

};

class AgentWrap : public Engine::Agent, public boost::python::wrapper<Engine::Agent>
{
public:
	AgentWrap( const std::string & id ) : Engine::Agent(id)
	{
	}

	void step()
	{
		this->get_override("step")();
	}	
	
	void serialize()
	{
		this->get_override("serialize")();
	}

	void * fillPackage()
	{
	}
};



class WorldWrap : public Engine::World, public boost::python::wrapper<Engine::World>
{
public:
	WorldWrap( const Engine::Simulation & simulation, const int & overlap, const bool & allowMultipleAgentsPerCell, const std::string & fileName ) : Engine::World( simulation, overlap, allowMultipleAgentsPerCell, fileName)
	{
	}

	void createRasters()
	{
		this->get_override("createRasters")();
	}	
	void createAgents()
	{
		this->get_override("createAgents")();
	}
	void stepEnvironment()
	{
		if (boost::python::override stepEnvironment = this->get_override("stepEnvironment"))
		{
			stepEnvironment();
			return;
		}
		Engine::World::stepEnvironment();
	}

	void default_StepEnvironment()
	{
		World::stepEnvironment();
	}
};

BOOST_PYTHON_MODULE(libpyPandora)
{
	boost::python::class_< Point2DInt >("Point2DIntStub", boost::python::init<const int & , const int & >() )
		.def_readwrite("_x", &Point2DInt::_x) 
		.def_readwrite("_y", &Point2DInt::_y) 
	;	
	
	boost::python::class_< StaticRasterWrap, boost::noncopyable >("StaticRasterStub")
		.def("resize", &Engine::StaticRaster::resize, &StaticRasterWrap::default_resize) 
		.def("getSize", &Engine::StaticRaster::getSize)
		.def("getValue", &Engine::StaticRaster::getValue, &StaticRasterWrap::default_getValue, boost::python::return_value_policy<boost::python::copy_const_reference>())
	;

	boost::python::class_< RasterWrap, boost::noncopyable >("RasterStub")
		.def("setInitValues", &Engine::Raster::setInitValues) 
		.def("setValue", &Engine::Raster::setValue)	
		.def("resize", &Engine::Raster::resize, &RasterWrap::default_resize) 
		.def("getSize", &Engine::Raster::getSize)
		.def("getValue", &Engine::Raster::getValue, &RasterWrap::default_getValue, boost::python::return_value_policy<boost::python::copy_const_reference>())
	;

	boost::python::class_< Engine::Simulation >("SimulationStub", boost::python::init< const int &, const int & >() )
	;

	boost::python::class_< WorldWrap, boost::noncopyable >("WorldStub", boost::python::init< const Engine::Simulation & , const int &, const bool &, const std::string & >() )
		.def("createRasters", boost::python::pure_virtual(&Engine::World::createRasters))
		.def("createAgents", boost::python::pure_virtual(&Engine::World::createAgents))
		.def("stepEnvironment", &Engine::World::stepEnvironment, &WorldWrap::default_StepEnvironment)
		.def("initialize", &Engine::World::initialize)
		.def("registerDynamicRaster", &Engine::World::registerDynamicRaster)
		.def("getDynamicRaster", &Engine::World::getDynamicRaster, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("run", &Engine::World::run)
		.def("addAgentStub", &WorldWrap::addAgent)
		.add_property("currentStep", &Engine::World::getCurrentStep)
	;
	
	boost::python::class_< AgentWrap, std::auto_ptr<AgentWrap>, boost::noncopyable >("AgentStub", boost::python::init< const std::string & > () )
		.def("step", boost::python::pure_virtual(&Engine::Agent::step))
		.def("serialize", boost::python::pure_virtual(&Engine::Agent::serialize))
		.add_property("id", boost::python::make_function(&Engine::Agent::getId, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.add_property("position", boost::python::make_function(&Engine::Agent::getPosition, boost::python::return_value_policy<boost::python::reference_existing_object>()), &Engine::Agent::setPosition )
	;
}

