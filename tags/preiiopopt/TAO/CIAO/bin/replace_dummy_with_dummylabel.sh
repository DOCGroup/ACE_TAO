#!/bin/bash
find . -name "*.mpc" > mpc_files
for i in `cat mpc_files`
do
  sed -e 's/requires += dummy$/requires += dummy_label/g' $i > tmp
  cat tmp > $i
done

rm -f mpc_files
rm -f tmp
