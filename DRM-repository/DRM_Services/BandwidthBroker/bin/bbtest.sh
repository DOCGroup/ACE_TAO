# !/bin/sh
echo java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $1 $2 $3 $4 $5 $6 $7 $8 $9
java mil.darpa.arms.mlrm.BandwidthBroker.bb.clients.BBTestSuite -ORBInitRef NameService=corbaloc::$ONS_HOST:$ONS_PORT/NameService $1 $2 $3 $4 $5 $6 $7 $8 $9


