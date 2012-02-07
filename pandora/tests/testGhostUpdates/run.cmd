#!/bin/bash
# @ job_name         = testGhostUpdates
# @ initialdir       = .
# @ output           = testGhostUpdates.log
# @ error            = testGhostUpdates.err
# @ total_tasks      = 4
# @ cpus_per_task    = 1
# @ tracing          = 0
# @ wall_clock_limit = 00:05:00

date
srun  ./testGhostUpdates
date
