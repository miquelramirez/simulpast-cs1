
#ifndef __Statistics_hxx__
#define __Statistics_hxx__

#include <boost/random.hpp>
#include <vector>

namespace Engine
{

class Statistics
{
	public:
	// to create random numbers using /dev/urandom
	//! Gets a random number from /dev/urandom to be used as a seed.
	uint64_t getNewSeed();
	private:
	typedef boost::mt19937 RandomEngine;
	RandomEngine _randomGenerator;
	static const int _distributionSize = 100000;

	// general random indexs
	boost::uniform_int<> _randomNumbers;
	boost::variate_generator< RandomEngine, boost::uniform_int<> > _nextRandomNumber;

	// TODO fix expo and normal distributions!
	std::vector<float> _exponentialDistribution;
	void generateExponentialDistribution();

	std::vector<float> _normalDistribution;
	void generateNormalDistribution();
public:
	Statistics();
	float getExponentialDistValue( float min, float max );
	float getNormalDistValue( float min, float max );

	// uniform dist does not need to generate numbers, as randomNumbers itself is a 
	int getUniformDistValue( int min, int max );
};

} // namespace Engine

#endif // __Statistics_hxx__

