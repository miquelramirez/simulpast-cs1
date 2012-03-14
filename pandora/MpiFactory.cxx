
#include "MpiFactory.hxx"

namespace Engine
{

MpiFactory * MpiFactory::_instance = 0;

MpiFactory * MpiFactory::instance()
{
	if(!_instance)
	{
		_instance = new MpiFactory();
	}
	return _instance;
}

MpiFactory::MpiFactory()
{
}

MpiFactory::~MpiFactory()
{
}

void MpiFactory::cleanTypes()
{
	for(TypesMap::iterator it=_types.begin(); it!=_types.end(); it++)
	{
		MPI_Datatype * type = it->second;
		MPI_Type_free(type);
	}
}

MpiFactory::TypesMap::iterator MpiFactory::beginTypes()
{
	return _types.begin();
}

MpiFactory::TypesMap::iterator MpiFactory::endTypes()
{
	return _types.end();
}

} // namespace Engine

