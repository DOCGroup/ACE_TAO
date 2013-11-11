eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$test->AddLibPath ('../lib');

$status = 0;
my $conf_suffix = "$PerlACE::svcconf_ext";

$svc_conf         = $test->LocalFile ("svc$conf_suffix");
$observer_conf    = $test->LocalFile ("observer$conf_suffix");
$svc_complex_conf = $test->LocalFile ("svc.complex$conf_suffix");
$mt_svc_conf      = $test->LocalFile ("mt.svc$conf_suffix");
$svc_complex_conf = $test->LocalFile ("svc.complex$conf_suffix");
$control_conf     = $test->LocalFile ("control$conf_suffix");

sub RunTest ($$$)
{
    my $message = shift;
    my $program = shift;
    my $arguments = shift;

    my $T = $test->CreateProcess ($program, $arguments);

    print STDERR "\n\n$message\n";

    my $test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 225);

    if ($test_status != 0) {
        print STDERR "ERROR: Test returned $test_status\n";
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
