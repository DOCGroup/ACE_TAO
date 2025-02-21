#!/bin/sh

. parameters

for j in copy_on_write copy_on_read delayed;
do
    for i in $NCONSUMERS;
    do
        ./compare_histo.pl ec_locking.$j.$i.16.histo.dat ec_locking.$j.32.16.histo.dat >$j.$i.dat;
    done;
done

