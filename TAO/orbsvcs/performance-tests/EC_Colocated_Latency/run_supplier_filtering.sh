#! /bin/sh
#
# $Id$
#

LOW_PRIORITY="1 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180"

for c in $LOW_PRIORITY; do
  date
  echo $c
  ./driver -ORBSvcConf ec.supplier_filter_null.conf -z -i 100000 -n $c > ec_supplier_filter_null.$c.txt 2>&1
  ./driver -ORBSvcConf ec.supplier_filter_per_supplier.conf -z -i 100000 -n $c > ec_supplier_filter_per_supplier.$c.txt 2>&1
done
