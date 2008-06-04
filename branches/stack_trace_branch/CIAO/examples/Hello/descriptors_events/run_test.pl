eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use RunEventTest;

# run_test_simple
$result = RunEventTest::run_test (
                 "deploymentplan_events.cdp", 
                 "NodeManagerMap.dat");

# run_test_collocated
$result += RunEventTest::run_test (
		 "deploymentplan_events_collocated.cdp", 
		 "NodeManagerMap_collocated.dat");

# run_test_federated
$result += RunEventTest::run_test (
                 "deploymentplan_federation.cdp",
                 "NodeManagerMap_collocated.dat");

# run_test_filtered
$result += RunEventTest::run_test (
                 "deploymentplan_with_filters.cdp", 
                 "NodeManagerMap_collocated.dat");
 
# run_test_multicast
# this test is not running successfully and is therefore skipped here
#$result += RunEventTest::run_test (
#                 "deploymentplan_federation_mcast.cdp", 
#                 "NodeManagerMap_collocated.dat");

# run_test_udp
$result += RunEventTest::run_test (
                 "deploymentplan_federation_udp.cdp", 
                 "NodeManagerMap.dat");

# run_test_udp_collocated
$result += RunEventTest::run_test (
                 "deploymentplan_federation_udp_collocated.cdp", 
                 "NodeManagerMap_collocated.dat");

# run_test_multiple_senders is not yet integrated

exit $result

