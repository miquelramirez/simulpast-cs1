
#include "CultivatedField.hxx"
#include "GujaratWorld.hxx"

namespace Gujarat
{

CultivatedField::CultivatedField( GujaratWorld & world, const Engine::Point2D<int> & position ) 
	: _position(position), _world(world),  _sown(false)
{
	_world.setValue("resourceType", _position, DOMESTICATED);
}

CultivatedField::~CultivatedField()
{
}

const Engine::Point2D<int> CultivatedField::getPosition()
{
	return _position;
}

void CultivatedField::sow()
{
	_sown = true;
}

void CultivatedField::harvest()
{
	_sown = false;
}

bool CultivatedField::isSown()
{
	return _sown;
}

bool  CultivatedField::requiresFallow()
{
	return _world.getValue("resourceType", getPosition())==FALLOW;
}

bool  CultivatedField::isDomesticated()
{
	return _world.getValue("resourceType", getPosition())==DOMESTICATED;
}

} // namespace Gujarat

