#!/bin/sh

if test "$OSTYPE" = "solaris"; then
  semids=`ipcs | grep $USER | awk '{ print ($2) }'`
  for p in $semids
    do ipcrm -s $p
  done
else
  # Linux/GNU
  ipcs -a | grep $USER | awk '{ print ($2) }' | xargs ipcrm sem;
fi

