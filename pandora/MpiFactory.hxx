
#ifndef __MpiFactory_hxx__
#define __MpiFactory_hxx__

#include <string>
#include <mpi.h>
#include <map>

namespace Engine
{
class Agent;

// this class is a factory method that creates Mpi related code
// source for the factory is automatically created from a python script in particular examples
class MpiFactory
{
public:
	typedef std::map< std::string, MPI_Datatype *> TypesMap;

private:
	static MpiFactory * _instance;

	MpiFactory();
	
	TypesMap _types;

public:
	static MpiFactory * instance();
	virtual ~MpiFactory();

	//! method defined by script that adds custom mpi types for each agent
	void registerTypes();
	//! method to delete from mpi stack all the created types
	void cleanTypes();

	void * createDefaultPackage( const std::string & type );
	Agent * createAndFillAgent( const std::string & type, void * package );

	TypesMap::iterator beginTypes();
	TypesMap::iterator endTypes();
};

} // namespace Engine

#endif // __MpiFactory_hxx__

