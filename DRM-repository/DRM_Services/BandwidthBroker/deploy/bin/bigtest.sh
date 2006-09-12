export DBHOST=pool2db
export  RES=`bin/isalive.sh access2`
. bin/bbtestext.sh a=add r=100 s=alpha d=golf q=highReliability p=3
. bin/bbtestext.sh a=add r=125 s=alpha d=echo q=highReliability p=3
. bin/bbtestext.sh a=add r=300 s=charlie d=hotel q=highReliability p=1
. bin/bbtestext.sh a=add r=900 s=delta d=kilo q=highReliability p=2
. bin/bbtestext.sh a=add r=700 s=kilo d=delta q=highReliability p=2
. bin/bbtestext.sh a=add r=250 s=lima d=golf q=highReliability p=3
. bin/bbtestext.sh a=add r=700 s=hotel d=juliet q=highReliability p=3
. bin/bbtestext.sh a=add r=350 s=alpha d=hotel q=highReliability p=3
. bin/bbtestext.sh a=add r=220 s=foxtrot d=bravo q=highReliability p=1
. bin/bbtestext.sh a=add r=330 s=delta d=golf q=highReliability p=4
. bin/bbtestext.sh a=add r=200 s=hotel d=kilo q=highReliability p=1
. bin/bbtestext.sh a=add r=300 s=kilo d=bravo q=highReliability p=5
. bin/bbtestext.sh a=add r=150 s=charlie d=golf q=highReliability p=9
. bin/bbtestext.sh a=add r=240 s=charlie d=lima q=highReliability p=4

