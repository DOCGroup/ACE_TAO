# !bin/sh
#export CCM_HOME=/usr/local/OpenCCM-0.8.1
#export JacORB_HOME=/opt/DRM/Middleware/JacORB2.1
export BROKER_ROOT=$DRM_ROOT/DRM_Services/BandwidthBroker/ccmtest
export CLASSPATH=
export CLASSPATH=${BROKER_ROOT}/proto/archives/Client.jar:

export CLASSPATH=$CLASSPATH:$BROKER_ROOT/proto/archives/OpenCCM_Plugins.jar
export CLASSPATH=$CLASSPATH:$OpenCCM_HOMEDIR/jar/OpenCCM_Runtime.jar
export CLASSPATH=$CLASSPATH:$OpenCCM_HOMEDIR/jar/OpenCCM_PropertyService.jar
export CLASSPATH=$CLASSPATH:$JACORB_HOME/lib/jacorb.jar
export CLASSPATH=$CLASSPATH:$JACORB_HOME/lib/logkit-1.2.jar
export CLASSPATH=$CLASSPATH:$JACORB_HOME/lib/concurrent-1.3.2.jar
export CLASSPATH=$CLASSPATH:$JACORB_HOME/lib/antlr-2.7.2.jar
export CLASSPATH=$CLASSPATH:$JACORB_HOME/lib/avalon-framework-4.1.5.jar

echo $CLASSPATH
java -cp $CLASSPATH  -DORBInitRef.NameService=file:$OpenCCM_CONFIG_DIR/NameService.IOR -Dorg.omg.CORBA.ORBClass=org.jacorb.orb.ORB -Dorg.omg.CORBA.ORBSingletonClass=org.jacorb.orb.ORBSingleton mil.darpa.arms.mlrm.BandwidthBroker.monitor.EventClient
