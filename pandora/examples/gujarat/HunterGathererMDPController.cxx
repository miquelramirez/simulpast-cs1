#include "HunterGathererMDPController.hxx"
#include "HunterGatherer.hxx"

namespace Gujarat
{

HunterGathererMDPController::HunterGathererMDPController( GujaratAgent* a, const HunterGathererMDPConfig& cfg )
	: AgentController( a ), _mdpConfig( cfg )
{
	_model = new HunterGathererMDPModel( dynamic_cast<HunterGatherer*>( a ) );
	_model->setup( cfg );
	_uctBasePolicy = new BasePolicy( *_model ); 
}

HunterGathererMDPController::~HunterGathererMDPController()
{
	delete _uctBasePolicy;
	delete _model;
}

Action*		HunterGathererMDPController::selectAction()
{
	_model->reset();

	UCT*	uctPolicy = new UCT( *_uctBasePolicy, 10, (unsigned)_mdpConfig.getHorizon(), 2.0f );

	Problem::action_t aIndex = (*uctPolicy)( _model->init() );

	Action* a = _model->init().availableActions(aIndex)->copy();
	
	delete uctPolicy;

	return a;
}

}
