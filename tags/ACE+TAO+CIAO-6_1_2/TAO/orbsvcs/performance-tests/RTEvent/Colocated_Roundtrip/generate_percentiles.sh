#/bin/sh
# $Id$

. parameters

PERCENTILES="0 .25 .50 .75 .99 .999 .9999 1"

for t in $*; do
  cat /dev/null > ${t}.avg.dat
  for p in $PERCENTILES; do
    cat /dev/null > ${t}$p.percentile.dat
  done

  for c in $NCONSUMERS; do
    echo $t $c
    INPUT=${t}.${c}.16.txt
    awk -v c=$c '/^HISTO/ {
                   n++;
                   s += $3
                 }
                 END {
                   printf("%d %f\n", c, s/n);
                 }' $INPUT >> ${t}.avg.dat

    TFILE=/tmp/${t}.$$
    awk '/^HISTO/ {print $3}' $INPUT | sort -n | uniq -c > $TFILE
    for p in $PERCENTILES; do
        awk -v N=$ITERATIONS '{print $2, $1 / N}' $TFILE |
        awk -v c=$c -v p=${p} '\
BEGIN { x = 0; l = 0; }
{ s += $2;
  l = $1;
  if (x == 0 && s >= p) {
      x = $1;
  }
}
END {
  if (s < p) {
    x = l;
  }
  printf("%d %f\n", c, x);
}' >> ${t}${p}.percentile.dat
    done
    /bin/rm -f $TFILE

  done

done
