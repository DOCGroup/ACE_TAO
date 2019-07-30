#!/bin/sh

for i in *.gz *.bz2 *.zip *.md5; do
  d=`echo $i | sed 's/\.[tz][ai][rp]/-6.5.7&/'`
  echo "Copying $i to $d"
  cp -ip $i $d
done

