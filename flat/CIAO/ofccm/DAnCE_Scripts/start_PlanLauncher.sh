#!/bin/bash

#launch plan launcher
$ACE_ROOT/bin/dance -l 2 -x file://$CIAO_ROOT/DAnCE/tests/DeploymentPlanDump/NewCcmDance1.cdp -w file://$CIAO_ROOT/DAnCE/tests/DeploymentPlanDump/test_res.cdr -ORBSvcConf logger.conf 

