#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
perf stat  -a -e 'power/energy-cores/, power/energy-gpu/, power/energy-pkg/, power/energy-ram/' build/fluid/fluid 1000 large.fld out.fld
perf stat  -a -e 'power/energy-cores/, power/energy-gpu/, power/energy-pkg/, power/energy-ram/' build/fluid/fluid 1000 small.fld out.fld