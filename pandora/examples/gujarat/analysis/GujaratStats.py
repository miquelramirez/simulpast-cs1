import os
import sys

class TimeStepData :
	
	def __init__( self, index ) :
		self.index = index
		self.action = None
		self.isStarving = None
		self.balanceAfterAction = None

class ActivityLog :

	def __init__( self ) :
		self.yearly = {}
		self.yearlyCounts = 0
		self.seasonal = {}
		self.seasonalCounts = {}
		for i in [0, 1, 2 ] :
			self.seasonal[i] = {}
			self.seasonalCounts[i] = 0

	def process( self, t ) :

		if 'move_home' in t.action :
			key = 'move_home'
		elif 'forage' in t.action :
			key = 'forage'
		elif 'do_nothing' in t.action :
			key = 'do_nothing'
		else :
			raise RuntimeError, "Unknown action %s found!"%t.action
		try :
			self.yearly[key] += 1
		except KeyError :
			self.yearly[key] = 1
		self.yearlyCounts += 1

		seasonIndex = (t.index % 360) / 120

		assert seasonIndex in [0,1,2]

		try :
			self.seasonal[seasonIndex][key] += 1
		except KeyError :
			self.seasonal[seasonIndex][key] = 1
		self.seasonalCounts[seasonIndex] += 1	

	def normalize( self, years ) :
		self.yearlyAvgDaysPerActivity = {}
		for key, value in self.yearly.iteritems() :
			self.yearlyAvgDaysPerActivity[ key ] = float(value)/float(self.yearlyCounts)
		self.seasonalAvgDaysPerActivity = {}
		for i in [0, 1, 2 ] :
			self.seasonalAvgDaysPerActivity[i] = {}
			self.seasonalAvgDaysPerActivity[i]['move_home'] = 0.0
			self.seasonalAvgDaysPerActivity[i]['forage'] = 0.0
			self.seasonalAvgDaysPerActivity[i]['do_nothing'] = 0.0
			for key, value in self.seasonal[i].iteritems() :
				self.seasonalAvgDaysPerActivity[ i ][key] = float(value) / float(self.seasonalCounts[i])



class NutritionLog :

	def __init__( self ) :
		self.yearlyStarvingCount = 0
		self.yearlyAvgBalance = 0.0
		self.yearlyCounts = 0
		self.seasonalStarving = {}
		self.seasonalAvgBalance = {}
		self.seasonalCounts = {}
		for i in [0, 1, 2 ] :
			self.seasonalStarving[i] = 0.0
			self.seasonalAvgBalance[i] = 0.0
			self.seasonalCounts[i] = 0
		

	def process( self, t ) :
	
		self.yearlyStarvingCount += t.isStarving
		self.yearlyAvgBalance += t.balanceAfterAction
		self.yearlyCounts += 1

		seasonIndex = (t.index % 360) / 120
		
		assert seasonIndex in [0,1,2]

		self.seasonalStarving[seasonIndex] += t.isStarving
		self.seasonalAvgBalance[seasonIndex] += float(t.balanceAfterAction)

		self.seasonalCounts[seasonIndex] += 1

	def 	normalize( self, years ) :
		self.yearlyStarvingCount /= float(years)
		self.yearlyAvgBalance = float(self.yearlyAvgBalance) / (float(self.yearlyCounts)*years)
		for i in [0,1,2] :
			self.seasonalStarving[i] /= float(years)
			self.seasonalAvgBalance[i] /= (float(self.seasonalCounts[i])*years)

class Agent :

	def __init__( self, log ) :
		if not os.path.exists( log ) :
			raise RuntimeError, "Cannot open log file: %s"%log
		self.name = None
		self.time_steps = []
		self.stats = {}
		self.__load( log )
		self.stats['activityLog'] = ActivityLog()
		self.stats['nutritionLog'] = NutritionLog()

	def __load( self, logfile ) :

		self.name = os.path.basename(logfile).split('.')[0]

		with open( logfile ) as instream :
			for line in instream :
				line = line.strip()
				key, value = line.split('=')
				if 'timestep' in key :
					t = TimeStepData( int(value) )
					self.time_steps.append( t )
					continue
				if 'action' in key :
					self.time_steps[-1].action = value
					continue
				if 'isStarvating' in key :
					if value == 'yes' :
						self.time_steps[-1].isStarving = 1
					else :
						self.time_steps[-1].isStarving = 0
					continue
				if 'collectedResourcesAfterConsumption' in key :
					self.time_steps[-1].balanceAfterAction = int(value)

	def computeStats( self ) :
		
		for statName, stat in self.stats.iteritems() :
			print >> sys.stdout, "Computing", statName, "statistics for agent", self.name
			for t in self.time_steps :
				stat.process( t )
			stat.normalize( len(self.time_steps)/360 )

class PopulationStats :

	def __init__( self, agentList ) :
		self.agents = agentList
		self.yearlyActivities = {}
		self.yearlyActivities['move_home'] = 0.0
		self.yearlyActivities['forage'] = 0.0
		self.yearlyActivities['do_nothing'] = 0.0
		self.seasonalActivities = {}
		for i in [0, 1, 2 ] :
			self.seasonalActivities[i] = {}
			self.seasonalActivities[i]['move_home'] = 0.0
			self.seasonalActivities[i]['forage'] = 0.0
			self.seasonalActivities[i]['do_nothing'] = 0.0

		self.yearlyStarvingCount = 0
		self.yearlyAvgBalance = 0.0
		self.yearlyMinStarvingCount = 360
		self.yearlyMaxStarvingCount = 0
		self.seasonalStarving = {}
		self.seasonalStarvingMin = {}
		self.seasonalStarvingMax = {}
		self.seasonalAvgBalance = {}
		for i in [0, 1, 2 ] :
			self.seasonalStarvingMin[i] = 360
			self.seasonalStarvingMax[i] = 0
			self.seasonalStarving[i] = 0.0
			self.seasonalAvgBalance[i] = 0.0

		self.__computeAggregateStats()
		self.__normalizeStats()

	def __computeAggregateStats( self ) :
		
		for agent in self.agents :
			for key, value in agent.stats['activityLog'].yearlyAvgDaysPerActivity.iteritems() :
				self.yearlyActivities[key] += float(value)
			self.yearlyStarvingCount += float(agent.stats['nutritionLog'].yearlyStarvingCount)
			self.yearlyAvgBalance += float(agent.stats['nutritionLog'].yearlyAvgBalance)
			if self.yearlyMinStarvingCount > agent.stats['nutritionLog'].yearlyStarvingCount :
				self.yearlyMinStarvingCount = agent.stats['nutritionLog'].yearlyStarvingCount
			if self.yearlyMaxStarvingCount < agent.stats['nutritionLog'].yearlyStarvingCount :
				self.yearlyMaxStarvingCount = agent.stats['nutritionLog'].yearlyStarvingCount
			for i in [0,1,2] :
				for key, value in agent.stats['activityLog'].seasonalAvgDaysPerActivity[i].iteritems() :
					self.seasonalActivities[i][key] += float(value)
				self.seasonalStarving[i] += float( agent.stats['nutritionLog'].seasonalStarving[i] )
				self.seasonalAvgBalance[i] += float( agent.stats['nutritionLog'].seasonalAvgBalance[i] )
				if self.seasonalStarvingMin[i] >  agent.stats['nutritionLog'].seasonalStarving[i] :
					self.seasonalStarvingMin[i] = agent.stats['nutritionLog'].seasonalStarving[i]
				if self.seasonalStarvingMax[i] < agent.stats['nutritionLog'].seasonalStarving[i] :
					self.seasonalStarvingMax[i] = agent.stats['nutritionLog'].seasonalStarving[i]

	def __normalizeStats( self ) :
		for key, value in self.yearlyActivities.iteritems() :
			self.yearlyActivities[key] /= float( len(self.agents) )
		self.yearlyStarvingCount /= float( len(self.agents) )
		self.yearlyAvgBalance /= float( len(self.agents) )
		for i in [0,1,2] :
			for key, value in self.seasonalActivities[i].iteritems() :
				self.seasonalActivities[i][key] /= float( len(self.agents) )
			self.seasonalStarving[i] /= float( len(self.agents) )
			self.seasonalAvgBalance[i] /= float( len(self.agents) )	

	def report( self, stream ) :

		print >> stream, "Population Statistics"
		print >> stream, "====================="
		print >> stream, "Yearly Time Distribution:"
		for key, value in self.yearlyActivities.iteritems() :
			print >> stream, "\t", key, "%.2f"%(value*100)
		print >> stream, "Seasonal Time Distribution:"
		for i in [0,1,2] :
			print >> stream, "\t", "Season #%d"%i
			for key, value in self.seasonalActivities[i].iteritems() :
				print >> stream, "\t\t", key, "%.2f"%(value*100)
		print >> stream, "Avg. Number of days starving (per year) : %.2f days"%self.yearlyStarvingCount
		print >> stream, "Min. Number of days starving (per year) : %d days"%self.yearlyMinStarvingCount
		print >> stream, "Max. Number of days starving (per year) : %d days"%self.yearlyMaxStarvingCount
		
		print >> stream, "Avg. Balance (per year): %.2f cal"%self.yearlyAvgBalance
		print >> stream, "Seasonal Nutrition Facts:"
		for i in [0,1,2] :
			print >> stream, "\t", "Season #%d"%i
			print >> stream, "\t\t", "Avg. Number of days starving: %.2f days"%self.seasonalStarving[i]
			print >> stream, "\t\t", "Min Number of days starving: %d days"%self.seasonalStarvingMin[i]
			print >> stream, "\t\t", "Max Number of days starving: %d days"%self.seasonalStarvingMax[i]

			print >> stream, "\t\t", "Avg. Balance: %.2f cal"%self.seasonalAvgBalance[i]

	
		
			
