#!/bin/bash
if [ 1 != $# ] ; then
  echo 'Usage: setenv.sh <path to the DRM-repository or try `pwd`>'
  return 1
fi
root=$1
if [ ! -d $root/DRM_Services ] ; then
  echo "$root does not appear to be a valid DRM-repository."
  return 1
fi

export DRM_ROOT=$root
echo "Setting DRM_ROOT=$DRM_ROOT"
export ARMS_ROOT=$DRM_ROOT/DRM_Services/BandwidthBroker/deploy
echo "Setting ARMS_ROOT=$ARMS_ROOT"
export SRC=$ARMS_ROOT/..
echo "Setting SRC=$SRC"

