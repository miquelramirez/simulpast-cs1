#!/bin/bash
# @ job_name         = testStringMpi
# @ initialdir       = .
# @ output           = testStringMpi.log
# @ error            = testStringMpi.err
# @ total_tasks      = 4
# @ cpus_per_task    = 1
# @ tracing          = 0
# @ wall_clock_limit = 00:05:00

date
srun  ./testStringMpi
date
