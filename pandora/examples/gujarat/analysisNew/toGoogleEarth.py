#!/usr/bin/python

import os

os.system('ogr2ogr -s_srs EPSG:24312 -t_srs EPSG:4326 gis/gujarat_latlong.shp gis/gujarat.shp')
os.system('ogr2ogr -f KML gis/gujarat.kml gis/gujarat_latlong.shp')

