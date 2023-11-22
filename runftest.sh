#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
chmod +x ftest/testFluid.sh
chmod +x ./ftest/ejecutables/fluid
chmod +x ./ftest/ejecutables/ftest
ftest/testFluid.sh