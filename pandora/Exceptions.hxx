
#ifndef __Exceptions_hxx__
#define __Exceptions_hxx__

#include <exception>
#include <string>

namespace Engine
{

class Exception : public std::exception
{
	std::string _error;
public:
	Exception( const std::string & error ) : _error(error){}
	virtual ~Exception() throw() {}
	virtual const char* what() const throw()
	{
		return _error.c_str();
	}

};

} // namespace Engine 

#endif // __Exceptions_hxx__

