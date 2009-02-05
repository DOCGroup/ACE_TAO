#!/bin/bash

dance --log-level 1 --exec-mgr -n MyHost -p -c -i -ORBListenEndpoints iiop://:12345 -ORBInitRef ProcessDestinationNC=corbaname::glasgow:22456#CDMW/SERVICES/ASSEMBLYANDDEPLOYMENT -x file://$CIAO_ROOT/DAnCE/tests/DeploymentPlanDump/NewCcmDance1.cdp -k corbaloc::glasgow:12345/ExecutionManager 


