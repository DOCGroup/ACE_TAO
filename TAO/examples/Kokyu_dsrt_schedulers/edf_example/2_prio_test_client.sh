#!/bin/bash

export PATH=/pkg/gnu/bin:$PATH
export ACE_ROOT=/home/yfzhang/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export LD_LIBRARY_PATH=$ACE_ROOT/ace:$ACE_ROOT/lib:$LD_LIBRARY_PATH
#export DATASTREAM_ROOT=/usr/src/kurt-2.4.18
export DATASTREAM_ROOT="/home/storri/kurt/kurt-2.4.18_12_01_2003"
export PYTHONPATH="$DATASTREAM_ROOT/src/dski/utils"

./dt_two_prio_client -N 2 -n 4

