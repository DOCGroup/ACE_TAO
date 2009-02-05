#!/bin/bash

#launch execution manager
#args=`$CIAO_ROOT/DAnCE/bin/integ_deployment.sh`
#$ACE_ROOT/bin/dance -l 1 -f em-log.conf 
dance --log-level 1 --exec-mgr em.ior -p -c -i -ORBListenEndpoints iiop://:1234666666 -ORBInitRef ProcessDestinationNC=corbaname::glasgow:22456#CDMW/SERVICES/ASSEMBLYANDDEPLOYMENT 


