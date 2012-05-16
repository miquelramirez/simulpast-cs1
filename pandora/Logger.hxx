
#ifndef __Logger_hxx__
#define __Logger_hxx__

#include <map>
#include <fstream>

namespace Engine
{

class Logger
{
	typedef std::map <std::string, std::ofstream *> FilesMap;
	static Logger * _instance;

	FilesMap _files;
protected:
	Logger();

public:
	static Logger & instance();
	virtual ~Logger();
	// message to file
	std::ofstream & log( const std::string & fileName );
};

} // namespace  Engine


#endif // __Logger_hxx__

