# !/bin/sh
java mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.client.NetworkMonitorEventClient $1  -ORBInitialHost $ONS_HOST -ORBInitialPort $ONS_PORT 
