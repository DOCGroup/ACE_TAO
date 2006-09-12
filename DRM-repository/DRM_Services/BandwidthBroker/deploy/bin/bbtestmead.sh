#!/bin/sh

export GID=22111
export MEAD_ROOT=/opt/DRM/Middleware/MEAD
export IP_GCS="192.168.120.101:34211,192.168.150.101:34211,";
if [ $GID ]; then
  # prevent java from re-exec'ing itself
  export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/i386/client:$JAVA_HOME/jre/lib/i386:$JAVA_HOME/jre/../lib/i386:/opt/DRM/Middleware/MEAD/spread-src-3.17.1/lib:$LD_LIBRARY_PATH
  export SPREAD_PORT=4803 
  export IS_STATELESS=yes
#  export IS_SERVER=yes
  export JACORB=yes
  export REPLICATION_STYLE=ACTIVE
  export IS_BB=yes
  # use MEAD::
  export PRELOADOPTS="env LD_PRELOAD=$MEAD_ROOT/replication/libmead.so.1"
  export GATEWAYOPTS="-DOAIAddr=pool3bb -DOAPort=$GID -Djava.net.preferIPv4Stack=true -Dorg.omg.CORBA.ORBClass=org.jacorb.orb.ORB -Dorg.omg.CORBA.ORBSingletonClass=org.jacorb.orb.ORBSingleton"
  echo "Using MEAD/Gateway and listening on port: $GID (with opts version)"
else
  export GATEWAYOPS="-DOAIAddr=pool2bb -DOAPort=11111"
fi


#echo java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $1 $2

echo "running: $PRELOADOPTS java $GATEWAYOPTS mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $*" 

#$PRELOADOPTS java $GATEWAYOPTS mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $*

java $GATEWAYOPTS mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $*



