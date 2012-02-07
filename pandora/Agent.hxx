
#ifndef __Agent_hxx__
#define __Agent_hxx__

#include "Point2D.hxx"
#include "World.hxx"

#include <string>
#include <sstream>


//#include "hdf5.h"

namespace Engine
{


//! Base class for all agents 
/*!
  This class encapsulates all the needed code to create a standard agent. Apart from id, main important attributes are the Position 
  of the agent inside space (in global coords) and an instance to the World that owns the agent.
*/

class Agent
{
protected:
	/** Agent identifier **/
	std::string _id;
	/** Flag to control if agent is "dead" or alive". it is used in analyzer in order to know if agent must be painted **/
	bool _exists;
	/** Position of the agent, in global coordinates **/
	Point2D<int> _position;
	/** Pointer to the world that owns this agent **/
	World * _world;
	
	/** *? pendent **/
	void serializeAttribute( const std::string & name, const int & value );
public:
	
	//! Standard constructor.
    /*! 
	  \param id id of the agent. It must be unique in the entire simulation.
    */
	Agent( const std::string & id );
	// els atribs de dalt segueixen : atribut\n comentari
	// perque els mètodes segueixen : comment\n metode ????
	// proposta : usar asserts 'require' i 'ensure'
	// exemple : 'require' afegit
	//! Standard constructor.
	/*! require :  
	  \param id id of the agent. It must be unique in the entire simulation.
	*/
	
	virtual ~Agent();

	const std::string & getId();

	bool exists();
	void setExists(bool exists);

	const Point2D<int> & getPosition();
	void setPosition( const Point2D<int> & position );

	virtual void step() = 0;
	// delete agent from world
	void remove();
	void setWorld( World * world );
	World * getWorld();
	friend std::ostream & operator<<( std::ostream & stream, Agent * agent );

	// TODO fix? when an agent is modified we need to alert the original in case of being an overlap copy
	//void interactionWithAgent(Agent * agent);
	virtual void * createPackage() = 0;
	// this function returns true if the type of the agent is the one passed by reference
	bool isType( const std::string & type );
	std::string getType();
	
	// defined in children, it must use serializeAttribute to save valuble data
	virtual void serialize()=0;
};

} // namespace Engine

#endif //__Agent_hxx__

