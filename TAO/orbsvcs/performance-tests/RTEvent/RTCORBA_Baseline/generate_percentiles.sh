#/bin/sh

CONSUMERS="1 2 4 6 8 10 12 14 16 18 20 22 24"

for t in reactive rtcorba; do
  cat /dev/null > ${t}.min.dat
  cat /dev/null > ${t}.avg.dat
  cat /dev/null > ${t}.99.dat
  cat /dev/null > ${t}.999.dat
  cat /dev/null > ${t}.9999.dat

  for c in $CONSUMERS; do
    INPUT=${t}.${c}.txt
    awk -v c=$c '/^HISTO/ {n++; s += $3} END {printf("%d %f\n", c, s/n);}' $INPUT >> ${t}.avg.dat
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

    for p in 99 999 9999; do
      awk '/^HISTO/ {print $3}' $INPUT | sort -n | uniq -c |
        awk '{print $2, $1 / 20000.0}' |
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

  done

done
