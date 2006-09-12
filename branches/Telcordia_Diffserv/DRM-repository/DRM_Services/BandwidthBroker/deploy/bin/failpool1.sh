#!/bin/bash

read 
echo "Failing pool at " `date` > /test/abc
/usr/bin/ssh pool2bb.bb-l2.arms.emulab.net > ~/log <<END
export DBHOST=pool2db.bb-l2.arms.emulab.net
echo "Failing pool at " `date` > ~/abc
cd /opt/DRM/DRM_Services/BandwidthBroker/deploy
bin/cl.sh 6 down
bin/cl.sh 4 down
bin/cl.sh 3 down
bin/cl.sh 7 down

# wait for routes to converge
sleep 6
java TimeStamp initial  > start_time;
# recompute routes
#bin/recomputeL3paths.sh

#echo java TimeStamp `cat start_time` > ~/bb_traceroute_time
#java TimeStamp < start_time >> ~/bb_traceroute_time


java TimeStamp initial > start_time
# invoke failure processing
export ONS_PORT=5001
export ONS_HOST=pool2bb
export ARMS_ROOT=.
export DRM_ROOT=/opt/DRM
. bin/setup-classpath.sh 
export DBHOST=pool2db
bin/bbtestmead.sh a=fault n=AdmissionControlBackup
#measure time
echo java TimeStamp `cat start_time` > ~/bb_failure_processing_time
java TimeStamp < start_time  >> ~/bb_failure_processing_time

bin/removeOldPaths.sh
END
echo " fail finished at" `date` >> /test/abc
echo "Failure processing complete "

#!/bin/bash

