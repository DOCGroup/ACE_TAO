#! /bin/sh

LOW_PRIORITY="1 20 40 60 80 100 120 140 160"

for c in $LOW_PRIORITY; do
  date
  echo $c
  ./driver -ORBSvcConf ec.supplier_filter_null.conf -z -i 50000 -n $c > ec_supplier_filter_null.$c.txt 2>&1
  ./driver -ORBSvcConf ec.supplier_filter_per_supplier.conf -z -i 50000 -n $c > ec_supplier_filter_per_supplier.$c.txt 2>&1
done
