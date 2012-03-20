
#ifndef __Simulation_hxx__
#define __Simulation_hxx__

namespace Engine
{

// this class encapulated the parameters defined to execute the simulation
class Simulation
{
	int _id;
	int _numTasks;
	int _size;
	int _numSteps;
	int _localRasterSize;
public:
	Simulation( const int & size, const int & numSteps );
	virtual ~Simulation();
	void init();

	const int & getId() const;
	const int & getNumTasks() const;
	const int & getSize() const;
	const int & getNumSteps() const;
	const int & getLocalRasterSize() const;
}; 

} // namespace Engine

#endif // __Simulation_hxx__

