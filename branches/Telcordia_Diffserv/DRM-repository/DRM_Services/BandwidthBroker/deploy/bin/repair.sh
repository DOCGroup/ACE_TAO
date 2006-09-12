
#!/bin/bash

export DBHOST=pool2db.bb-l2.arms.emulab.net
bin/cl.sh 6 up
bin/cl.sh 4 up
bin/cl.sh 3 up
bin/cl.sh 7 up

ssh access9.bb-l2.arms.emulab.net /proj/arms/etc/ospf/mkrouter.sh
ssh access7.bb-l2.arms.emulab.net /proj/arms/etc/ospf/mkrouter.sh
ssh access4.bb-l2.arms.emulab.net /proj/arms/etc/ospf/mkrouter.sh
ssh access5.bb-l2.arms.emulab.net /proj/arms/etc/ospf/mkrouter.sh
