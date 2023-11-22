#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
perf stat build/fluid/fluid 1 large.fld out.fld
perf stat build/fluid/fluid 50 large.fld out.fld
perf stat build/fluid/fluid 100 large.fld out.fld
perf stat build/fluid/fluid 300 large.fld out.fld
perf stat build/fluid/fluid 500 large.fld out.fld
perf stat build/fluid/fluid 800 large.fld out.fld
perf stat build/fluid/fluid 1000 large.fld out.fld
perf stat build/fluid/fluid 1500 large.fld out.fld