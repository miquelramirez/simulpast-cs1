
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


#include "Config.hxx"
#include "Exceptions.hxx"
#include <sstream>
#include <string.h>

#define CONSTCONFIG_TYPEINT 	0
#define CONSTCONFIG_TYPESTRING 	1

//**********************************************************
  Config::Config() 
      : _path("")
      ,_numSteps(0)
      {
//      std::cout << "Config::Config()" << std::endl;
      }  
//**********************************************************
//**********************************************************
  Config::Config(const std::string & filename)  
    : _path("")
    , _numSteps(0)
    { 
//    std::cout << "Config::Config(filename)" << std::endl;  
    //deserialize(filename);   
    };
//**********************************************************
  Config::~Config() {}
//**********************************************************
//**********************************************************
  void Config::serialize(const std::string & filename)
    {  
    
    }
//**********************************************************
 TiXmlElement *Config::openTiXml(const std::string & filename)
 //TiXmlDocument *Config::openTiXml(const std::string & filename)
    {
    doc = new TiXmlDocument(filename.c_str());
    if (!doc->LoadFile())
      {
      std::stringstream oss;
      oss << "Exception, error at open for file " << filename << std::endl;
      throw Engine::Exception(oss.str());
      }
    
    TiXmlHandle hDoc(doc);    
    TiXmlHandle hRoot(0);

    TiXmlElement *pRoot;
    pRoot = doc->FirstChildElement( "config" );
    
    return pRoot;    
    }
//**********************************************************
  void Config::closeTiXml()
    {
      delete doc;      
      doc = 0;
    }
//**********************************************************
  void Config::extractAttribs(TiXmlElement *pRoot)
    {

    TiXmlElement *pParm;
    
	pParm = pRoot->FirstChildElement("path");
	_path = pParm->Attribute("value");
	pParm = pRoot->FirstChildElement("numSteps");
	_numSteps = atoi(pParm->Attribute("value"));
    
    extractParticularAttribs(pRoot);
    }
//**********************************************************
  void Config::deserialize(const std::string & filename)
    {    
    TiXmlElement *pRoot = openTiXml(filename);
    
    if ( pRoot )
      {
      extractAttribs(pRoot);    
      // destroy pRoot and pParm
      closeTiXml();
      }//if      
    }


  const int & Config::getNumSteps() const
{
	return _numSteps;
}
//**********************************************************

    /*
    extract scenario:
    
    <resource_map type="AAA" }>
	<AAA value="BBB"/>
    </resource_map>
    
    AAA={raster_file,distrib,peak}

    TODO:

    <resource_map type="CCC" }>
	<CCC value="DDD"/>
	<CCC value="EEE"/>
	<CCC value="FFF"/>
	<CCC value="GGG"/>
	<CCC value="HHH"/>
    </resource_map>
    
    */
    
//**********************************************************

/* USAGE EXAMPLE :
int main(int argc, char *argv[])
  {
  std::cout << "begin tinyxml test" << std::endl;
  //std::stringstream oss;  
  config c;
  c.deserialize("config.xml");  
    
  std::cout << "config>" << c << std::endl;
  }
*/  
  
  
  
