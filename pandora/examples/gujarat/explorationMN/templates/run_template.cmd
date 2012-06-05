#!/bin/bash
# @ job_name         = gujaratINDEX
# @ initialdir       = ./INITIALDIR
# @ output           = gujarat.log
# @ error            = gujarat.err
# @ total_tasks      = 4
# @ cpus_per_task    = 4
# @ tracing          = 0
# @ wall_clock_limit = 05:00:00

date
srun  ../../gujarat
date
