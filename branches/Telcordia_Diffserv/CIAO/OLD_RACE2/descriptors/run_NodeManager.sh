#!/bin/sh
sudo killall NodeManager
sudo killall NodeApplication
if [ -f /opt/path.sh ]; then
   source /opt/path.sh
else
   echo "Not found /opt/path.sh"
fi
host=`hostname`
$CIAO_ROOT/DAnCE/NodeManager/NodeManager -ORBEndpoint iiop://$host:50000 -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication -d 30
