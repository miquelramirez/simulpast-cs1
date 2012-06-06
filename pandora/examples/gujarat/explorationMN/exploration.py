#!/usr/bin/python
import fileinput, sys, os

#mapSizes = ['400','800','1200','1600']

numExecutions = 10
mapSizes = ['400']
numHGs = ['1','10','50','500']
#controllers = ['Random','DecisionTree','MDP']
controllers = ['Random']

xmlTemplate = 'templates/config_template.xml'
runTemplate = 'templates/run_template.cmd'

mapSizeKey = 'MAPSIZE'
numHGKey = 'NUMHG'
controllerKey = 'CTYPE'

indexKey = 'INDEX'
initialDirKey = 'INITIALDIR'


def replaceKey( fileName, key, value ):
	for line in fileinput.FileInput(fileName,inplace=1):
		if key in line:
			line = line.replace(key, value)
		sys.stdout.write(line)
	fileinput.close()

os.system('rm -rf results_*')

index = 0
print 'creating test workbench'
for mapSize in mapSizes:
	for numHG in numHGs:
		for controller in controllers:
			for numExecution in numExecutions:
				print 'creating gujarat instance: ' + str(index) + ' for map: ' + mapSize + ' numHG: ' + numHG + ' with controller: ' + controller + ' and execution: ' + numExecution
				dirName = 'results_size'+mapSize+'_numHG'+numHG+'_controller_'+controller+'_ex'+numExecution
				os.system('mkdir '+dirName)
				configName = dirName + '/config.xml'			
				os.system('cp '+xmlTemplate+' '+configName)
				replaceKey(configName, mapSizeKey, mapSize)
				replaceKey(configName, numHGKey, numHG)
				replaceKey(configName, controllerKey, controller)

				runName = dirName+'/run.cmd'
				os.system('cp '+runTemplate+' '+runName)
				replaceKey(runName, indexKey, str(index))
				replaceKey(runName, initialDirKey, dirName)
				index += 1

print 'workbench done, submitting tasks'
index = 0
for mapSize in mapSizes:
	for numHG in numHGs:
		for controller in controllers:	
			for numExecution in numExecutions:
				print 'submitting gujarat instance: ' + str(index) + ' for map: ' + mapSize + ' numHG: ' + numHG + ' with controller: ' + controller + ' and execution: ' + numExecution
				dirName = 'results_size'+mapSize+'_numHG'+numHG+'_controller_'+controller+'_ex'+numExecution
				os.system('mnsubmit '+dirName+'/run.cmd')
				index += 1

