
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

	const int & getId();
	const int & getNumTasks();
	const int & getSize();
	const int & getNumSteps();
	const int & getLocalRasterSize();
}; 

} // namespace Engine

#endif // __Simulation_hxx__

