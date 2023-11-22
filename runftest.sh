#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
chmod +x ftest/testFluid.sh
ftest/testFluid.sh ./build/fluid/fluid ./build/ftest/ftest
