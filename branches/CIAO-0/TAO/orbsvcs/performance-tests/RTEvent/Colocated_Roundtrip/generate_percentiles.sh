#/bin/sh

. parameters

for t in $TYPES; do
  cat /dev/null > ${t}.min.dat
  cat /dev/null > ${t}.avg.dat
  cat /dev/null > ${t}.99.dat
  cat /dev/null > ${t}.999.dat
  cat /dev/null > ${t}.9999.dat

  for c in $NCONSUMERS; do
    echo $t $c
    INPUT=${INPUT_BASE}.${t}.${c}.8.txt
    awk -v c=$c '/^HISTO/ {
                   n++;
                   s += $3
                 }
                 END {
                   printf("%d %f\n", c, s/n);
                 }' $INPUT >> ${t}.avg.dat
    awk -v c=$c 'BEGIN { n = 0; }
                 /^HISTO/ {
                  if (n == 0 || m > $3) {
                    m = $3;
                  }
                  n++;
                }
                END {
                  printf("%d %f\n", c, m);
                }' $INPUT >> ${t}.min.dat

    TFILE=/tmp/${t}.$$
    awk '/^HISTO/ {print $3}' $INPUT | sort -n | uniq -c > $TFILE
    for p in 99 999 9999; do
        awk -v N=$ITERATIONS '{print $2, $1 / N}' $TFILE |
        awk -v c=$c -v p=0.${p} '\
BEGIN { x = 0; }
{ s += $2;
  if (x == 0 && s > p) {
      x = $1;
  }
}
END {
  printf("%d %f\n", c, x);
}' >> ${t}.${p}.dat
    done
    /bin/rm -f $TFILE

  done

done
