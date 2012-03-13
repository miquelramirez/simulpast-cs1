#!/usr/bin/python
import sys


def writeRegisterTypes( f, listAgents ):
	f.write('void MpiFactory::registerTypes()\n')
	f.write('{\n')	
	for i in range(0, len(listAgents)):
		f.write('\t_types.insert( std::make_pair( "'+listAgents[i]+'", create'+listAgents[i]+'Type()));\n')
	f.write('}\n')
	f.write('\n')

def writeCreateDefaultPackage( f, listAgents ):
	f.write('void * MpiFactory::createDefaultPackage( const std::string & type )\n')
	f.write('{\n')
	for i in range(0, len(listAgents)):
		f.write('\tif(type.compare("'+listAgents[i]+'")==0)\n')
		f.write('\t{\n')
		f.write('\t\treturn new '+listAgents[i]+'Package;\n')
		f.write('\t}\n')
	f.write('\n')
	f.write('\tstd::stringstream oss;\n')
	f.write('\toss << "MpiFactory::createDefaultPackage - unknown agent type: " << type;\n')
	f.write('\tthrow Engine::Exception(oss.str());\n')
	f.write('\treturn 0;\n')
	f.write('}\n')
	f.write('\n')
	return None

def writeCreateAndFillAgents( f, listAgents, namespaces ):
	f.write('Agent * MpiFactory::createAndFillAgent( const std::string & type, void * package )\n')
	f.write('{\n')
	for i in range(0, len(listAgents)):
		f.write('\tif(type.compare("'+listAgents[i]+'")==0)\n')
		f.write('\t{\n')
		f.write('\t\treturn new '+namespaces[i]+"::"+listAgents[i]+'(package);\n')
		f.write('\t}\n')
	f.write('\n')
	f.write('\tstd::stringstream oss;\n')
	f.write('\toss << "MpiFactory::createDefaultPackage - unknown agent type: " << type;\n')
	f.write('\tthrow Engine::Exception(oss.str());\n')
	f.write('\treturn 0;\n')


	f.write('}\n')
	f.write('\n')
	return None

def getMpiTypeAttribute( typeAttribute ):
	mpiTypeAttribute = 'MPI_INT'
	if(typeAttribute=='float'):
		mpiTypeAttribute = 'MPI_FLOAT'
	return mpiTypeAttribute

def writeCreateType( f, nameAgent, attributesMap ):
	# we have to send 4 basic attributes (_id, _position._x, _position._y & _exists + the number of dynamic attributes
	numAttributes = 4+len(attributesMap)
	f.write('MPI_Datatype * create'+nameAgent+'Type()\n')
	f.write('{\n')
	f.write('\t'+nameAgent+'Package package;\n')
	f.write('\n')
	# lengths and types
	f.write('\tint blockLengths['+str(numAttributes)+'];\n')
	f.write('\tMPI_Datatype typeList['+str(numAttributes)+'];\n')
	f.write('\t// block lengths and types\n')
	# id, position & exists
	f.write('\t// _id\n')
	f.write('\tblockLengths[0] = 32;\n');
	f.write('\ttypeList[0] = MPI_CHAR;\n');
	f.write('\t// _position._x\n')
	f.write('\tblockLengths[1] = 1;\n')
	f.write('\ttypeList[1] = MPI_INT;\n')
	f.write('\t// _position._y\n')
	f.write('\tblockLengths[2] = 1;\n')
	f.write('\ttypeList[2] = MPI_INT;\n')
	f.write('\t// _exists\n')
	f.write('\tblockLengths[3] = 1;\n')
	f.write('\ttypeList[3] = MPI_INT;\n')

	# dynamic params
	index = 4
	for nameAttribute, typeAttribute in attributesMap.items():
		f.write('\t// '+nameAttribute+'\n')
		# TODO vectors and lists?
		f.write('\tblockLengths['+str(index)+'] = 1;\n')
		mpiTypeAttribute = getMpiTypeAttribute(typeAttribute)
		f.write('\ttypeList['+str(index)+'] = '+mpiTypeAttribute+';\n')
		index = index+1
	f.write('\n')
	# displacements
	f.write('\tMPI_Aint displacements['+str(numAttributes)+'];\n')
	f.write('\tMPI_Aint startAddress;\n')
	f.write('\tMPI_Aint address;\n')
	
	# id, position & exists
	f.write('\t// id\n')
	f.write('\tMPI_Address(package._idMpi, &startAddress);\n')
	f.write('\tdisplacements[0] = 0;\n')

	f.write('\t// _position._x\n')
	f.write('\tMPI_Address(&package._positionMpi._x, &address);\n')
	f.write('\tdisplacements[1] = address-startAddress;\n')

	f.write('\t// _position._y\n')
	f.write('\tMPI_Address(&package._positionMpi._y, &address);\n')
	f.write('\tdisplacements[2] = address-startAddress;\n')

	f.write('\t// _exists\n')
	f.write('\tMPI_Address(&package._existsMpi, &address);\n')
	f.write('\tdisplacements[3] = address-startAddress;\n')

	# dynamic params
	index = 4
	for nameAttribute, typeAttribute in attributesMap.items():
		f.write('\t// '+nameAttribute+'\n')
		f.write('\tMPI_Address(&package.'+nameAttribute+'Mpi, &address);\n')
		f.write('\tdisplacements['+str(index)+'] = address-startAddress;\n')
		index = index+1
	
	# create mpi data type	
	f.write('\n')
	f.write('\tMPI_Datatype * newDataType = new MPI_Datatype;\n')
	f.write('\tMPI_Type_struct('+str(numAttributes)+', blockLengths, displacements, typeList, newDataType);\n')
	f.write('\tMPI_Type_commit(newDataType);\n')
	f.write('\treturn newDataType;\n')
	
	f.write('}\n')
	f.write('\n')
	return None

def createFactoryMethods( listAgents, factoryFile, namespaces, listAttributesMaps ):
	f = open(factoryFile, 'w')
	print '\tcreating mpi factory: ' + factoryFile
	# headers
	f.write('\n')
	f.write('#include "MpiFactory.hxx"\n')
	f.write('#include "Exceptions.hxx"\n')
	f.write('\n')
	for i in range(0, len(listAgents)):
		print '\t\tadding: ' + listAgents[i] + ' to factory file: ' + factoryFile
		f.write('#include "'+listAgents[i]+'.hxx"\n')
		f.write('#include "'+listAgents[i]+'_mpi.hxx"\n')
	f.write('\n')
	f.write('namespace Engine\n')
	f.write('{\n')
	f.write('\n')
	
	for i in range(0, len(listAgents)):
		writeCreateType( f, listAgents[i], listAttributesMaps[i])
	writeRegisterTypes( f, listAgents )
	writeCreateDefaultPackage( f, listAgents )
	writeCreateAndFillAgents( f, listAgents, namespaces )

	# close header & namespace
	f.write('} // namespace Engine\n')	
	f.write('\n')	
	f.close()
	return None

def createMpiHeader( agentName, source, header, attributesMap ):
	print '\t\tcreating mpi header: mpiCode/'+agentName+'_mpi.hxx for agent: ' + agentName + ' from source: ' + source + ' and header: ' + header
	f = open('mpiCode/'+agentName+'_mpi.hxx', 'w')
	# header
	f.write('\n')
	f.write('#ifndef __'+agentName+'_mpi_hxx\n')
	f.write('#define __'+agentName+'_mpi_hxx\n')
	f.write('\n')
	f.write('#include "Point2D.hxx"\n')
	f.write('\n')
	# struct Package
	f.write('typedef struct\n')
	f.write('{\n')
	# basic data: _id, _position & _exists
	f.write('\tchar _idMpi[32];\n')
	f.write('\tbool _existsMpi;\n')
	f.write('\tEngine::Point2D<int> _positionMpi;\n')
	f.write('\n')
	# dynamic params
	for nameAttribute, typeAttribute in attributesMap.items():
		f.write('\t'+typeAttribute+' '+nameAttribute+'Mpi;\n')
	# close struct
	f.write('} '+agentName+'Package;\n')
	# close file
	f.write('\n')
	f.write('#endif // __'+agentName+'_mpi_hxx\n')
	f.write('\n')
	f.close()
	return None

def writeFillPackage( f, agentName, attributesMap ):
	f.write('void * '+agentName+'::fillPackage()\n')
	f.write('{\n')
	# basic params: _id, _position & _exists
	f.write('\t'+agentName+'Package * package = new '+agentName+'Package;\n')
	f.write('\tmemcpy(&package->_idMpi, _id.c_str(), sizeof(char)*_id.size());\n')
	f.write('\tpackage->_idMpi[_id.size()] = \'\\0\';\n')
	f.write('\tpackage->_existsMpi = _exists;\n')
	f.write('\tpackage->_positionMpi = _position;\n')
	f.write('\n')
	# dynamic params
	for nameAttribute in attributesMap.keys():
		f.write('\tpackage->'+nameAttribute+'Mpi = '+nameAttribute+';\n')
	f.write('\treturn package;\n')
	f.write('}\n')
	f.write('\n')
	return None

def writeConstructor( f, agentName, parent , attributesMap ):
	f.write(agentName+'::'+agentName+'( void * package ) : '+parent+'((('+agentName+'Package*)package)->_idMpi)\n')
	f.write('{\n')
	f.write('\t'+agentName+'Package * particularPackage = ('+agentName+'Package*)package;\n')
	# basic params: _position & _exists
	f.write('\t_exists = particularPackage->_existsMpi;\n')
	f.write('\t_position = particularPackage->_positionMpi;\n')
	f.write('\n')
	# dynamic params
	for nameAttribute in attributesMap.keys():
		f.write('\t'+nameAttribute+' = particularPackage->'+nameAttribute+'Mpi;\n')
	f.write('}\n')
	f.write('\n')
	return None

def createMpiCode( agentName, source, header, namespace, parent, attributesMap ):
	print '\t\tcreating mpi file: mpiCode/'+agentName+'_mpi.cxx for agent: ' + agentName + ' from source: ' + source + ' and header: ' + header
	f = open('mpiCode/'+agentName+'_mpi.cxx', 'w')
	# header
	f.write('\n')
	f.write('#include "'+agentName+'_mpi.hxx"\n')
	f.write('#include "'+agentName+'.hxx"\n')
	f.write('#include <cstring>\n')
	f.write('#include <mpi.h>\n')
	f.write('\n')
	f.write('namespace '+namespace+'\n')
	f.write('{\n')
	f.write('\n')
	writeFillPackage( f, agentName, attributesMap )
	writeConstructor( f, agentName, parent, attributesMap )
	f.write('} // namespace '+namespace+'\n')
	f.write('\n')
	f.close()
	return None

def addAttribute( line, key, attributesMap ):
	splitLine = line.split()
	# 1st word will be the type of the attribute
	typeAttribute = splitLine[0]
	# 2nd word will be the name, removing final ';'
	nameAttribute = splitLine[1]
	nameAttribute = nameAttribute.strip(';')
	attributesMap[nameAttribute] = typeAttribute
	print '\t\t\tattribute detected: ' + nameAttribute + ' with type: ' + typeAttribute
	return None

def detectAttributes( headerName ):
	print '\t\tlooking for attributes in header: '+ headerName + '...'
	f = open(headerName, 'r')
	key = 'MpiAttribute'
	attributesMap = {}
	for line in f:
		if line.find(key) != -1:
			addAttribute( line, key, attributesMap )
	f.close()
	return attributesMap

# TODO attributes in parent class?
def execute( target, source, env ):
	print 'generating code for mpi communication...'
	listAgents = []
	listAttributesMaps = []
	namespaceAgents = env['namespaces']
	parents = env['parents']
	for i in range(1,len(source)):
		sourceName = str(source[i])
		headerName = sourceName.replace(".cxx", ".hxx")
		listAgents += [sourceName.replace(".cxx", "")]
		print '\tprocessing agent: ' + listAgents[i-1]
		# get the list of attributes to send/receive in MPI
		attributesMap = detectAttributes(headerName)
		# create header declaring a package with the list of attributes
		createMpiHeader(listAgents[i-1], sourceName, headerName, attributesMap )
		# create a source code defining package-class copy
		createMpiCode(listAgents[i-1], sourceName, headerName, namespaceAgents[i-1], parents[i-1], attributesMap )
		listAttributesMaps.append(attributesMap)

	# fill mpi code registering types and additional methods
	createFactoryMethods(listAgents, str(target[0]), namespaceAgents, listAttributesMaps )
	print 'done!'
	return None

