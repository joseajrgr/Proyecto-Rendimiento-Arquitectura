#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
build/utest/utest