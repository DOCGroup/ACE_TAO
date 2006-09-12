#!/bin/sh
java mil.darpa.arms.mlrm.BandwidthBroker.networkmonitor.client.NetworkMonitorClient  -ORBInitialHost $ONS_HOST -ORBInitialPort $ONS_PORT 
