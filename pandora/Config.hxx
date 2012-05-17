
/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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

