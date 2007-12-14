eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use RunEventTest;

exit RunEventTest::run_test (
       "deploymentplan_events_collocated.cdp", 
       "NodeManagerMap_collocated.dat");
