
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include "Point2D.hxx"
#include "StaticRaster.hxx"
#include "Raster.hxx"
#include "Simulation.hxx"
#include "World.hxx"
#include "Agent.hxx"

typedef Engine::Point2D<int> Point2DInt;

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
	
	boost::python::class_< Engine::StaticRaster >("StaticRasterStub")
		.def("resize", &Engine::StaticRaster::resize) 
		.def("getSize", &Engine::StaticRaster::getSize)
		.def("getValue", &Engine::Raster::getValue, boost::python::return_value_policy<boost::python::reference_existing_object>())
	;
	
	boost::python::class_< Engine::Raster >("RasterStub")
		.def("setInitValues", &Engine::Raster::setInitValues) 
		.def("setValue", &Engine::Raster::setValue)
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

