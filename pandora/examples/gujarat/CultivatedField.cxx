
#include "CultivatedField.hxx"
#include "GujaratWorld.hxx"

namespace Gujarat
{

CultivatedField::CultivatedField( GujaratWorld & world, const Engine::Point2D<int> & position ) : _world(world), _position(position), _sown(false)
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

} // namespace Gujarat

