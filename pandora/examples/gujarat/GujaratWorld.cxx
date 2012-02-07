
#include "GujaratWorld.hxx"
#include "Raster.hxx"
#include "Point2D.hxx"
#include "HunterGatherer.hxx"
#include "AgroPastoralist.hxx"
#include "Exceptions.hxx"
#include "Statistics.hxx"
#include "GujaratConfig.hxx"
#include <limits>

namespace Gujarat
{

GujaratWorld::GujaratWorld( Engine::Simulation & simulation, const GujaratConfig & config ) 
	: World(simulation, 1, true, config._path+"/ned.h5"), _agentKey(0), _climate(config), _config(config)					
{
	// overlap is maxHomeRange + 1 to allow splits to be in adjacent worlds
	// TODO code a function proces config for resources 
}

GujaratWorld::~GujaratWorld()
{
}    

void GujaratWorld::createRasters()
{
	registerStaticRaster("soils", true);
	getStaticRaster("soils").loadGDALFile(_config._soilFile, *this);

	registerStaticRaster("dem", true);
	getStaticRaster("dem").loadGDALFile(_config._demFile, *this);
	
	registerDynamicRaster("moisture", true);
	getDynamicRaster("moisture").setInitValues(0, std::numeric_limits<int>::max(), 0);
	registerDynamicRaster("resources", true); // DEBUG Resources will be generated with an explicit function
	getDynamicRaster("resources").setInitValues(0, std::numeric_limits<int>::max(), 0);
	
	registerDynamicRaster("resourceType", true); // type of resources: wild, domesticated or fallow
	getDynamicRaster("resourceType").setInitValues(0, SEASONALFALLOW, 0);
	registerDynamicRaster("consecutiveYears", false); // years passed using a given cell for a particular use
	getDynamicRaster("consecutiveYears").setInitValues(0, 3, 0);

	Engine::Point2D<int> index;
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)
		{
			setValue("moisture", index, 0);
			setValue("resources", index, 0);
			setValue("consecutiveYears", index, 0);
			setValue("resourceType", index, WILD);
		}
	}
	updateMoisture();
}

void GujaratWorld::createAgents()
{
	for(int i=0; i<_config._numHG; i++)
	{ 
		if((i%_simulation.getNumTasks())==_simulation.getId())
		{
			std::ostringstream oss;
 			oss << "HunterGatherer_" << i;
			HunterGatherer * agent = new HunterGatherer(oss.str());
			agent->setPosition(getRandomPosition());
			addAgent(agent);
			agent->moveHome();
			std::cout << _simulation.getId() << " new HunterGathrer: " << agent << std::endl;
		}
	}

	for(int i=0; i<_config._numAP; i++)
	{ 
		if((i%_simulation.getNumTasks())==_simulation.getId())
		{
			std::ostringstream oss;
 			oss << "AgroPastoralist_" << i;
			AgroPastoralist * agent = new AgroPastoralist(oss.str());
			agent->setPosition(getRandomPosition());
			addAgent(agent); 
			agent->moveHome();
			std::cout << _simulation.getId() << " new AgroPastoralist: " << agent << std::endl;
		}
	}
}

/*------------------------------*/

float GujaratWorld::moistureFunction( const Soils & soilType, const float & rain, const Seasons & season )
{
//proposal: posar un objecte soil a la cel.la
//proposal: una subclass per cada tipus de soil i cadascuna implementa el comportament diferent: evaporacio, absorcio aigua,...
//proposal: put moisture, soilType inside Soil object (one for each cell)    
/* PENDENT
    proposal: 
    define a class array constant containing the alphas called moistureSoilAlpha[][]
    float m[3][3]={{0.0,.25,.5},{.0,.5,1.0},{.0,.75,1.5}};
    retrieve alpha with moistureSoilAlpha[soilType][season]
    moistureFunction( sT, r, s, mR) = r - meanRain*moistureSoilAlpha[soilType][season];
*/
	float newMoisture; // newMoisture = r - alpha * R;
	float alpha = 0.0;
	switch (soilType)
	{
		case WATER:
			switch (season)
			{
				case HOTWET:
					alpha = 0.0; // dropable case, initialization could cover it.
					break;
				case COLDDRY:
					alpha = 0.25; 
					break;
				case HOTDRY:
					alpha = 0.5;
			}	
			break;

		case INTERDUNE:
			switch (season)
			{
				case HOTWET:
					alpha = 0.0; // dropable case, initialization could cover it.
					break;
				case COLDDRY:
					alpha = 0.5;
					break;
				case HOTDRY:
					alpha = 1.0;
			}
			break;
		
		case DUNE:
			switch (season)
			{
				case HOTWET:
					alpha = 0.0; // dropable case, initialization could cover it.
					break;
				case COLDDRY:
					alpha = 0.75;
					break;
				case HOTDRY:
					alpha = 1.5;
			}
	}
	//std::cout << "step: " << _step << " alpha: " << alpha << " with season: " << season << " and soil type: " << soilType << std::endl;
	newMoisture = std::max(0.0f, rain - (alpha * _climate.getMeanAnnualRain()));
	return newMoisture;
}

/*------------------------------*/

void GujaratWorld::updateRainfall()
{		
	_climate.step();
	if(_step%3==HOTWET)
	{
		std::cout << _climate.getRain() << std::endl;
		//std::cout << "rain fall for year: " << _step/3 << " is: " << _climate.getRain() << std::endl;
	}
}

void GujaratWorld::updateMoisture()
{
	float rain = _climate.getRain();
	Seasons season = _climate.getSeason();

	//std::cout << "in step: " << _step << " season: " << season << std::endl;
	//foreach c in cells
	//	c.moisture = moistureFunction( c.soilType, rain, s, m);

	Engine::Point2D<int> index;
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)			
		{
			/*
			int value    = getValue(key, index);
			int maxValue = getMaxValue(key, index);
			*/
			Soils soilT = (Soils)getValue("soils", index);
			float moisture = moistureFunction( soilT, rain, season );
			setValue("moisture", index, moisture);
		}
	}
}

void GujaratWorld::updateSoilCondition()
{
	Engine::Point2D<int> index;
	if(_climate.getSeason()==HOTWET)
	{
		for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
		{
			for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)
			{
				setValue("resources", index, getValue("moisture", index));
				if(getValue("resourceType", index)==WILD)
				{
					continue;
				}
				
				if(getValue("resourceType", index)==SEASONALFALLOW)
				{
					setValue("resourceType", index, DOMESTICATED);
				}
				int consecutiveYears = getValue("consecutiveYears", index);
				consecutiveYears++;				
				if(consecutiveYears<3)
				{
					setValue("consecutiveYears", index, consecutiveYears);
				}
				else
				{
					setValue("consecutiveYears", index, 0);
					if(getValue("resourceType", index)==FALLOW)
					{
						setValue("resourceType", index, WILD);

					}
					else
					{
						setValue("resourceType", index, FALLOW);
					}
				}
			}
		}
	}
	else if(_climate.getSeason()==HOTDRY)
	{
		for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
		{
			for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)
			{
				if(getValue("resourceType", index)==DOMESTICATED)
				{
					setValue("resourceType", index, SEASONALFALLOW);
				}
			}
		}
	}
}

void GujaratWorld::updateResources()
{
	Engine::Point2D<int> index;
	for(index._x=_boundaries._origin._x; index._x<_boundaries._origin._x+_boundaries._size._x; index._x++)		
	{
		for(index._y=_boundaries._origin._y; index._y<_boundaries._origin._y+_boundaries._size._y; index._y++)
		{
			setValue("resources", index, getValue("moisture", index)/10);
		}
	}
}

void GujaratWorld::stepEnvironment()
{
	// at the end of simulation
	_climate.advanceSeason();

	// update rainfall and moisture
	updateRainfall();
	updateMoisture();
	updateSoilCondition();
	updateResources();
}

Engine::Agent * GujaratWorld::createAgentFromPackage( const std::string & type, void * package )
{
	/*
	if(type.compare("neandertal")==0)
	{
		GujaratHuntGathPackage * formattedPackage = 0;
		try
		{
			formattedPackage = (GujaratHuntGathPackage*)package;
		}
		catch(std::exception & e)
		{
			std::cout << "GujaratWorld::createAgentFromPackage, wrong cast with exception: " << e.what() << std::endl;
		}
		return (Engine::Agent*)(new GujaratHuntGath(*formattedPackage));
	}
	else if(type.compare("sapiens")==0)
	{
		GujaratAgroPastPackage * formattedPackage = 0;
		try
		{
			formattedPackage = (GujaratAgroPastPackage*)package;
		}
		catch(std::exception & e)
		{
			std::cout << "GujaratWorld::createAgentFromPackage, wrong cast with exception: " << e.what() << std::endl;
		}
		return (Engine::Agent*)(new GujaratAgroPast(*formattedPackage));
	}
	else
	{	
		std::stringstream oss;
		oss << "GujaratWorld::createAgentFromPackage - unknown agent type: " << type;
		throw Engine::Exception(oss.str());
	}
	*/
	return 0;
}

void * GujaratWorld::createPackage( const std::string & type )
{
	/*
	if(type.compare("neandertal")==0)
	{
		return new GujaratHuntGathPackage;
	}
	else if(type.compare("sapiens")==0)
	{
		return new GujaratAgroPastPackage;
	}
	std::stringstream oss;
	oss << "GujaratWorld::createPackage - unknown agent type: " << type;
	throw Engine::Exception(oss.str());
	*/
	return 0;
}

void GujaratWorld::registerTypes()
{
	/*
	_types.insert( std::make_pair( "HunterGatherer", createTypeGujaratHuntGath())); // TODO use HG and AP ???
	_types.insert( std::make_pair( "AgroPastoralist", createTypeGujaratAgroPast()));
	*/
}

MPI_Datatype * GujaratWorld::createTypeGujaratHuntGath()
{
	/*
	GujaratHuntGathPackage package;
	
	int blockLengths[16];
	MPI_Datatype typelist[16];

	// block lengths and types
	// 32 chars
	blockLengths[0] = 32;
	typelist[0] = MPI_CHAR;	
	for(int i=1; i<16; i++)
	{
		blockLengths[i] = 1;
		typelist[i] = MPI_INT;
	}

	MPI_Aint displacements[16];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;
	MPI_Address(&package._resources, &address);
	displacements[3] = address-startAddress;
	MPI_Address(&package._exists, &address);

	displacements[4] = address-startAddress;
	MPI_Address(&package._population, &address);
	displacements[5] = address-startAddress;
	MPI_Address(&package._homeRange, &address);
	displacements[6] = address-startAddress;
	MPI_Address(&package._cannibalism, &address);

	displacements[7] = address-startAddress;
	MPI_Address(&package._stepsUsingCannibalism, &address);
	displacements[8] = address-startAddress;
	MPI_Address(&package._age, &address);
	displacements[9] = address-startAddress;
	MPI_Address(&package._splitChance, &address);
	displacements[10] = address-startAddress;
	MPI_Address(&package._children, &address);

	displacements[11] = address-startAddress;
	MPI_Address(&package._minForkResources, &address);
	displacements[12] = address-startAddress;
	MPI_Address(&package._splitWaste, &address);
	displacements[13] = address-startAddress;
	MPI_Address(&package._maxAge, &address);
	displacements[14] = address-startAddress;
	MPI_Address(&package._minFoodToEat, &address);
	displacements[15] = address-startAddress;
	MPI_Address(&package._minFoodToMove, &address);
	
	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(11, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
	return newDataType;
	*/
	return NULL;
}

MPI_Datatype * GujaratWorld::createTypeGujaratAgroPast()
{
	/*
	GujaratAgroPastPackage package;
	
	int blockLengths[9];
	MPI_Datatype typelist[9];

	// block lengths and types
	// 32 chars
	blockLengths[0] = 32;
	typelist[0] = MPI_CHAR;	
	for(int i=1; i<9; i++)
	{
		blockLengths[i] = 1;
		typelist[i] = MPI_INT;
	}

	MPI_Aint displacements[9];
	displacements[0] = 0;

	MPI_Aint startAddress;
	MPI_Aint address;
	MPI_Address(package._id, &startAddress); 

	MPI_Address(&package._position._x, &address);
	displacements[1] = address-startAddress;
	MPI_Address(&package._position._y, &address);
	displacements[2] = address-startAddress;
	MPI_Address(&package._resources, &address);
	displacements[3] = address-startAddress;
	MPI_Address(&package._exists, &address);

	displacements[4] = address-startAddress;
	MPI_Address(&package._population, &address);
	displacements[5] = address-startAddress;
	MPI_Address(&package._homeRange, &address);

	displacements[6] = address-startAddress;
	MPI_Address(&package._age, &address);
	displacements[7] = address-startAddress;
	MPI_Address(&package._splitChance, &address);
	displacements[8] = address-startAddress;
	MPI_Address(&package._children, &address);

	MPI_Datatype * newDataType = new MPI_Datatype;
	MPI_Type_struct(9, blockLengths, displacements, typelist, newDataType);
	MPI_Type_commit(newDataType);
	return newDataType;
	*/
	return NULL;
}

long int GujaratWorld::getNewKey()
{
	return _agentKey++;
}
	
const Climate & GujaratWorld::getClimate() const
{
	return _climate;
}

} // namespace Gujarat

