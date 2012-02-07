
#ifndef CONFIGCLASS
#define CONFIGCLASS 1

#include <iostream>
#include <cstdlib>
#include <tinyxml.h>
#include <string>

namespace NED {
//class NED::NEDWorld;
    class NEDWorld;
    };
   
    
class Config
  { 
  //private:
  TiXmlDocument *doc;
  
public:
  std::string _path;
  int _numSteps;
 
  
//----------------------------------  
  
  public:

  Config();  
  Config(const std::string & filename);  
  virtual ~Config();
  
  // -- methods -----------------
  TiXmlElement *openTiXml(const std::string & filename);  
  void closeTiXml();
  virtual void extractAttribs(TiXmlElement *pRoot);
  virtual void extractParticularAttribs(TiXmlElement *pRoot) = 0;    
  virtual void serialize(const std::string & filename);
  virtual void deserialize(const std::string & filename);
  
  friend std::ostream & operator<<( std::ostream & stream, const Config & c)
      {
      return stream << "Config(" 
			<< c._path
		<< "," << c._numSteps << ")";
      }

  const int & getNumSteps() const;
   
  };//class
    
#endif  

/*
TODO 
config : config assign | assign;

assign : var list_value | var value;

var : var_name type;

value : int | string | bool | list_value;

list_value : list_value value | value;

*/

