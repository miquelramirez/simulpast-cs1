#include "AgentController.hxx"
#include <sstream>

namespace Gujarat
{

AgentController::AgentController( GujaratAgent* a )
	: _agent( a )
{
	std::stringstream fNameStream;
	fNameStream << a->getId();
	fNameStream << ".controller.log";
	
	_log = new std::ofstream( fNameStream.str().c_str() );	

}

AgentController::~AgentController()
{
	_log->close();
}

}
