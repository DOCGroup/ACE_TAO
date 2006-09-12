#!/bin/bash

for i in {1..2}; do
    # add a flow
    FLOWID=`bin/bbtest.sh a=add r=200 q=highReliability s=alpha d=bravo p=3 |
            perl -ne 'if (/flowID:(\d+).*admitted/) {print "$1\n";}'`

    #delete the flow
    bin/bbtest.sh a=del f=$FLOWID
done
