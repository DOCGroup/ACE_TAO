#!/bin/sh

if [ -z "$USER" ]; then
  USER=`id | sed 's/).*//; s/.*(//'`
fi

SYSTEM=`uname -s`
case "$SYSTEM" in
  "Linux" )
    ipcs -a | grep $USER | awk '{ print ($2) }' | xargs ipcrm sem;
    ;;
  * )
    semids=`ipcs | grep "^s" | grep $USER | awk '{ print ($2) }'`
    for p in $semids
      do ipcrm -s $p
    done
    ;;
esac
