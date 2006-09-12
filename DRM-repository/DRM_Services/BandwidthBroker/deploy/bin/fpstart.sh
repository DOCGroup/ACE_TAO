
echo java mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.FlowProvisioningServer cfg/BandwidthBroker/ioslog.prop cfg/BandwidthBroker/routerAuth pool1 -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService 

java mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server.FlowProvisioningServer p=cfg/BandwidthBroker/ioslog.prop r=cfg/BandwidthBroker/routerAuth l=yes f=FP1 w=0 a=0 -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService < bin/p &
