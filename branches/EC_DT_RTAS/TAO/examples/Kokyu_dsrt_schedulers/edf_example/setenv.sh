#!/bin/bash

export ACE_ROOT="/usr/src/ACE_CS520"
export TAO_ROOT="$ACE_ROOT/TAO"
export LD_LIBRARY_PATH="$ACE_ROOT/ace:$TAO_ROOT/examples/Kokyu_dsrt_schedulers"
export DATASTREAM_ROOT="/usr/src/kurt-2.4.18"

./$1
