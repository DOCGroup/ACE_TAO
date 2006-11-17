#!/bin/sh
pgrep -l NodeApplication

if [ $# == 1 ] ; then
  pgrep NodeApplication | xargs -n 1 sudo chrt -p -f $1 
else
  pgrep NodeApplication | xargs -n 1 sudo chrt -p -f 50 
fi

pgrep NodeApplication | xargs -n 1 sudo chrt -p  

