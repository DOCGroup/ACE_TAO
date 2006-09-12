ssh -T $1 > temp <<ENDROUTE
traceroute $2
ENDROUTE
hops=`awk '/192\.168/ { for (i=1; i < 100; i ++) { if (match ($i, "192.168")) {printf ("%s ", substr ($i, 2, length($i)-2)); break;} }}' temp`
echo bin/update_l3_paths.sh $1 $2 $hops
bin/update_l3_paths.sh $1 $2 $hops
