#!/bin/sh
INET=`./show-inet.sh | tr ':' ' ' | gawk 'NR==2{print $3}'`
echo INET=$INET

LAST_DIGIT=`echo $INET | tr '.' ' ' | gawk '{print $2}'`
#echo LAST_DIGIT=$LAST_DIGIT

NODE_MAN_PORT=5000$LAST_DIGIT
echo NODE_MAN_PORT=$NODE_MAN_PORT

NODE_APP_IORFILE=NodeApp$LAST_DIGIT.ior
echo NODE_APP_IORFILE=$NODE_APP_IORFILE

NODE_MAN_IORFILE=NodeMan$LAST_DIGIT.ior
echo NODE_MAN_IORFILE=$NODE_MAN_IORFILE

SQ="'"
#echo SQ=$SQ

DQ="\""
#echo DQ=$DQ

COMMAND_LINE="$CIAO_ROOT/DAnCE/NodeManager/NodeManager \
-ORBEndpoint iiop://$INET:$NODE_MAN_PORT \
-s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication \
-o $NODE_MAN_IORFILE \
-a '\"-ORBEndpoint\" \"iiop://$INET\" \"-o\" \"$NODE_APP_IORFILE\"' "

echo $COMMAND_LINE
#`echo $COMMAND_LINE`
    
