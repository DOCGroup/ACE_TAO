
cp cfg/BandwidthBroker/nmconfig.prop.emulab cfg/BandwidthBroker/nmconfig.prop
cp cfg/BandwidthBroker/bbconfig.prop.emulab cfg/BandwidthBroker/bbconfig.prop

echo java mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.NetworkMonitorServer $NETMONLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService 
java mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.server.NetworkMonitorServer $NETMONLOGPROP -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService 
