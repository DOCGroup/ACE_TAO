# !/bin/sh
echo java mil.darpa.arms.mlrm.BandwidthBroker.bb.server.BBServer $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService &
java mil.darpa.arms.mlrm.BandwidthBroker.bb.server.BBServer $BBLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService &
