#!/bin/sh

CCM_HOME=/home/vb/openccm
export CCM_HOME

JacORB_HOME=/home/vb/JacORB
export JacORB_HOME

BROKER_ROOT=/home/vb/Telcordia
export BROKER_ROOT

CPATH=
CPATH=$BROKER_ROOT/proto/archives/BandwidthBroker.jar
CPATH=$CPATH:$BROKER_ROOT/proto/archives/OpenCCM_Plugins.jar
CPATH=$CPATH:$CCM_HOME/openccm/JacORB-2.1/jar/OpenCCM_Runtime.jar
CPATH=$CPATH:$JacORB_HOME/lib/jacorb.jar
CPATH=$CPATH:$JacORB_HOME/lib/logkit-1.2.jar
CPATH=$CPATH:$JacORB_HOME/lib/concurrent-1.3.2.jar
CPATH=$CPATH:$JacORB_HOME/lib/antlr-2.7.2.jar
CPATH=$CPATH:$JacORB_HOME/lib/avalon-framework-4.1.5.jar
export CPATH

java -classpath $CPATH -DORBInitRef.NameService=file:$HOME/OpenCCM_CONFIG_DIR/NameService.IOR -Dorg.omg.CORBA.ORBClass=org.jacorb.orb.ORB -Dorg.omg.CORBA.ORBSingletonClass=org.jacorb.orb.ORBSingleton mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.client.FlowProvisioningClient

