#!/usr/bin/python
import os
import sys
import matplotlib.pyplot as plt

def acquire_avg_resources_from_log( logfile ) :

	data = []
	timesteps = []
	max_r = 0
	max_t = 0

	with open( logfile) as instream :

		for line in instream :
			line = line.strip()
			key, value = line.split('=')
			if 'timestep' in key :
				timesteps.append( int(value) )
				if int(value) > max_t :
					max_t = int(value)
			if 'avgCurrent' in key :
				data.append( float(value) )
				if float(value) > max_r :
					max_r = float(value)

	return  data, max_t, max_r

def main() :
	
	if len(sys.argv) < 2 :
		print >> sys.stderr, "No input file was specified"
		print >> sys.stderr, "Usage: ./plot-avg-resources.py <world log>"
		sys.exit(1)

	data, xmax, ymax = acquire_avg_resources_from_log( sys.argv[1] )
	
	plt.plot( data )
	plt.title( 'GujaratSim: Yearly Cell Resources (Pop=1)' )
	plt.xlabel( 'Timestep' )
	plt.ylabel( 'Avg. Resources' )	
	plt.axis( [ 0, xmax, 0, ymax*1.10 ] )	

	plt.show()
	

if __name__ == '__main__' :
	main()
