#!/bin/bash
# @ job_name         = testUpdateOverlapData
# @ initialdir       = .
# @ output           = testUpdateOverlapData.log
# @ error            = testUpdateOverlapData.err
# @ total_tasks      = 4
# @ cpus_per_task    = 1
# @ tracing          = 0
# @ wall_clock_limit = 00:05:00

date
srun  ./testUpdateOverlapData
date
