
#ifndef __CultivatedField_hxx__
#define __CultivatedField_hxx__

#include "Point2D.hxx"

namespace Gujarat
{
class GujaratWorld;

class CultivatedField
{
	Engine::Point2D<int> _position;
	GujaratWorld & _world;
	bool _sown;
public:
	CultivatedField( GujaratWorld & world, const Engine::Point2D<int> & position );
	virtual ~CultivatedField();
	void sow();
	void harvest();
	bool isSown();
	const Engine::Point2D<int> getPosition();
};

} // namespace Gujarat

#endif // __CultivatedField_hxx__

