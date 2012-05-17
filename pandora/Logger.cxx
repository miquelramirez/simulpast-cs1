
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

#include "Logger.hxx"

#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>

namespace Engine
{

Logger * Logger::_instance = 0;

Logger & Logger::instance()
{
	if(!_instance)
	{
		_instance = new Logger;
		// create dir where logs will be stored if it is not already created
		boost::filesystem::create_directory("logs");
	}
	return *_instance;
}

Logger::Logger()
{
}

Logger::~Logger()
{
	for(FilesMap::iterator it=_files.begin(); it!=_files.end(); it++)
	{
		 std::ofstream * file = it->second;
		 file->close();
		 delete file;
	}

}

std::ofstream & Logger::log( const std::string & key )
{
	FilesMap::iterator it = _files.find(key);
	std::ofstream * file = 0;
	// create a new file if it is closed
	if(it==_files.end())
	{
		std::stringstream fileName;
		fileName<< "logs/" << key<< ".log";

		_files.insert( make_pair( key, new std::ofstream( fileName.str().c_str() )));
		it = _files.find(key);
	}
	file = it->second;
	return *file;
}
} // namespace Engine
