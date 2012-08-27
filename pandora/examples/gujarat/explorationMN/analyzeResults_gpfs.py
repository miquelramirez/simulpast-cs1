#!/usr/bin/python
import os

numExecutions = 4
mapSizes = ['1200']
numHGs = ['100']
controllers = ['DecisionTree']

os.system('rm -rf results')
os.system('mkdir results')

for numExecution in range(0,numExecutions):
	for mapSize in mapSizes:
		for numHG in numHGs:
			for controller in controllers:
				fileName = '/gpfs/projects/bsc21/bsc21887/data_controller'+controller+'_size'+str(mapSize)+'_hg'+str(numHG)+'_ex'+str(numExecution)+'/gujarat.h5'
				print 'analyzing results file: ' + fileName + ' for instance with: controller: ' + controller + ' size: ' + str(mapSize) + ' numHG: ' + str(numHG) + ' and execution: ' + str(numExecution)
				os.system('../analysisNew/statisticsGujarat '+fileName+' agents.csv rasters.csv')
				os.system('mv agents.csv agents_controller'+controller+'_size'+str(mapSize)+'_hg'+str(numHG)+'_ex'+str(numExecution)+'.csv')
#os.system('mv resources.csv resources_size'+mapSize+'_numHG'+numHG+'_controller_'+controller+'_ex'+str(numExecution))

