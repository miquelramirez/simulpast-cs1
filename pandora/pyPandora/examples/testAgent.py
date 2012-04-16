#!/usr/bin/python

import sys
sys.path.append('../')
sys.path.append('../../')

from pyPandora import Simulation, Agent, World, Point2DInt

class MyAgent(Agent):
	def __init__(self, id):
		Agent.__init__( self, id)

	def step(self):
		self.position._x = self.position._x+1
		self.position._y = self.position._y+1

	def serialize(self):
		return

class MyWorld(World):
	def __init__(self, simulation ):
		World.__init__( self, simulation)

	def createRasters(self):		
		self.registerDynamicRaster("test", 1)
		self.getDynamicRaster("test").setInitValues(0, 0, 0)

	def createAgents(self):
		for i in range (0, 10):
			newAgent = MyAgent('id_'+str(i))
			newAgent.position = Point2DInt(i, i)
			self.addAgent(newAgent)

mySimulation = Simulation(32, 10)
myWorld = MyWorld(mySimulation)
myWorld.initialize()
myWorld.run()

