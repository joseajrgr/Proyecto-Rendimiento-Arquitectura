#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
perf stat build/fluid/fluid 1 small.fld out.fld
perf stat build/fluid/fluid 50 small.fld out.fld
perf stat build/fluid/fluid 100 small.fld out.fld
perf stat build/fluid/fluid 300 small.fld out.fld
perf stat build/fluid/fluid 500 small.fld out.fld
perf stat build/fluid/fluid 800 small.fld out.fld
perf stat build/fluid/fluid 1000 small.fld out.fld
perf stat build/fluid/fluid 1500 small.fld out.fld
perf stat build/fluid/fluid 2500 small.fld out.fld
perf stat build/fluid/fluid 5000 small.fld out.fld