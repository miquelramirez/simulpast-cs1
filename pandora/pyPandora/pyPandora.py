#!/usr/bin/python

import sys
sys.path.append('../')
import libpyPandora

class Point2DInt(libpyPandora.Point2DIntStub):
	def __init__(self, x, y):
		libpyPandora.Point2DIntStub.__init__(self, x, y)

class StaticRaster(libpyPandora.StaticRasterStub):
	def __init__(self):
		libpyPandora.StaticRasterStub.__init__(self)

class Raster(libpyPandora.RasterStub):
	def __init__(self):
		libpyPandora.RasterStub.__init__(self)

class Simulation(libpyPandora.SimulationStub):
	def __init__(self, size, numSteps):
		libpyPandora.SimulationStub.__init__(self, size, numSteps )

class Agent(libpyPandora.AgentStub):
	def __init__(self, id):
		libpyPandora.AgentStub.__init__( self, id)

class World(libpyPandora.WorldStub):
	_listOfAgents = []
	def __init__(self, simulation ):
		libpyPandora.WorldStub.__init__( self, simulation, 0, 1, "data/testWorld.h5")

	def addAgent(self, agent):
		self.addAgentStub(agent)
		self._listOfAgents.append(agent)

