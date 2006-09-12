tc -s filter show dev eth0 root > filter-data ; awk '/handle/ {h = $10; split (h, hh, "x"); printf ("%s ", hh[2])} /overlimits/ {b = $9 ; split (b, v, ")"); print v[1]} ' filter-data; rm filter-data

