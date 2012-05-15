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

MDPAction * HunterGathererMDPController::selectAction()
{
	log() << "timestep=" << agentRef().getWorld()->getCurrentTimeStep() << std::endl;
	log() << "\tagent.position=" << agentRef().getPosition() << std::endl;
	
	_model->reset();

	UCT*	uctPolicy = new UCT( *_uctBasePolicy, 
	(unsigned)_mdpConfig.getWidth(), (unsigned)_mdpConfig.getHorizon(), _mdpConfig.getExplorationBonus(), false );

	Problem::action_t aIndex = (*uctPolicy)( _model->init() );

	MDPAction* a = _model->init().availableActions(aIndex)->copy();
	
	delete uctPolicy;

	log() << "\taction_selected=";
	a->describe( log() );
	log() << std::endl; 
	
	return a;
}

}
