#!/bin/bash

export PATH=/pkg/gnu/bin:$PATH
export ACE_ROOT=/home/ron/yfzhang/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export LD_LIBRARY_PATH=$ACE_ROOT/ace:$ACE_ROOT/lib:$LD_LIBRARY_PATH
export DATASTREAM_ROOT=/usr/local
#export PYTHONPATH=$DATASTREAM_ROOT/src/dski/utils

./dt_two_prio_client
