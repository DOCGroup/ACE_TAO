tfile=`mktemp temp.XXXXXX`

r1Alive=`bin/isalive.sh $1`
r2Alive=`bin/isalive.sh $2`

if [ "$r1Alive" == "1"  -a "$r2Alive" == "1" ]; then
echo Finding routes
ssh -T $1 > $tfile <<ENDROUTE
traceroute $3
ENDROUTE
echo $tfile
cat $tfile
hops=`awk '/192\.168/ { for (i=1; i < 100; i ++) { if (match ($i, "192.168")) {printf ("%s ", substr ($i, 2, length($i)-2)); break;} }}' $tfile`
echo bin/update_l3_paths.sh $1 $2 $hops
fi
bin/update_l3_paths.sh $1 $2 $hops
rm $tfile
