eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$svc_conf = PerlACE::LocalFile ("svc.conf");
$observer_conf = PerlACE::LocalFile ("observer.conf");
$svc_complex_conf = PerlACE::LocalFile ("svc.complex.conf");
$mt_svc_conf = PerlACE::LocalFile ("mt.svc.conf");
$svc_complex_conf = PerlACE::LocalFile ("svc.complex.conf");
$control_conf = PerlACE::LocalFile ("control.conf");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $TEST = new PerlACE::Process ($program, $arguments);

    print STDERR "\n\n$message\n";
    
    my $test = $TEST->SpawnWaitKill (240);

    if ($test != 0) {
        print STDERR "ERROR: Test returned $test\n";
        $status = 1;
    }
}

RunTest ("Reconnect suppliers and consumers, using disconnect/connect calls",
         "Reconnect",
         "-ORBsvcconf $svc_conf -suppliers 100 -consumers 100 -d 100");

RunTest ("Reconnect suppliers and consumers, using connect calls",
         "Reconnect",
         "-ORBsvcconf $svc_conf -suppliers 100 -consumers 100 -d 100 -s -c");

RunTest ("Shutdown EC with clients still attached",
         "Shutdown",
         "-ORBsvcconf $svc_conf -suppliers 5 -consumers 5");

RunTest ("Gateway test",
         "Gateway",
         "-ORBsvcconf $observer_conf");

RunTest ("Complex event channel test, multiple ECs connected through gateways",
         "Observer",
         "-ORBsvcconf $observer_conf -consumer_tshift 0 -consumers 5 -supplier_tshift 0 -suppliers 2 -burstsize 10 -burstcount 10 -burstpause 0");

RunTest ("Timeout tests",
         "Timeout",
         "-ORBsvcconf $svc_conf");

RunTest ("Wildcard tests",
         "Wildcard",
         "-ORBsvcconf $svc_conf");

RunTest ("Negation tests",
         "Negation",
         "-ORBsvcconf $svc_conf");

RunTest ("Bitmask tests",
         "Bitmask",
         "-ORBSvcConf $svc_complex_conf");

RunTest ("Disconnect callbacks test",
         "Disconnect",
         "-ORBsvcconf $svc_conf");

RunTest ("MT Disconnects test",
         "MT_Disconnect",
         "-ORBSvcConf $mt_svc_conf");

RunTest ("Atomic Reconnection test",
         "Atomic_Reconnect",
         "-ORBSvcConf $mt_svc_conf");

RunTest ("Complex filter",
         "Complex",
         "-ORBSvcConf $svc_complex_conf");

RunTest ("Control test",
         "Control",
         "-ORBSvcConf $control_conf");

RunTest ("Random test",
         "Random",
         "-ORBSvcConf $svc_conf -suppliers 4 -consumers 4 -max_recursion 1");

exit $status;
