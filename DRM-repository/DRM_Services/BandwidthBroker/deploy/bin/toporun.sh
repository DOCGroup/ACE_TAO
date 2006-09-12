# !/bin/sh
echo "java mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.TopologyPopulator <xml-input> cfg/BandwidthBroker/xmltopo.prop cfg/BandwidthBroker/bbconfig.db.prop"
echo $1
java mil.darpa.arms.mlrm.BandwidthBroker.topologyDB.TopologyPopulator $1 ${TOPOLOGPROP} ${DBCONFIG}
#$Id$
