eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################

use strict;
use Sys::Hostname;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
my $debug_level = 0;
my $imr_debug_level = 0;
my $delay = 0;
my $got_object_not_exist_exception = 0;

foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $imr_debug_level = '1';
    }
    if ($i eq '-forwardalways') {
        $delay = '5';
        $forward_opt = "-ORBForwardInvocationOnObjectNotExist 1";
    }
    if ($i eq '-forwardonce') {
        $delay = '5';
        $forward_opt = "-ORBForwardOnceOnObjectNotExist 1";
        $got_object_not_exist_exception = 1;
    }
}

my $imr_locator_ior = PerlACE::LocalFile ("imr_locator.ior");
my $protocol = "iiop";
my $host = hostname();
my $port = PerlACE::uniqueid () + 10001;
my $imr_endpoint = "-ORBEndpoint " . "$protocol" . "://:" . $port;
my $imr_db = PerlACE::LocalFile ("imr.db");
# -ORBDebugLevel 10 -ORBVerboseLogging 1 -ORBLogFile imr.log 
my $imr_locator_args = "$imr_endpoint -UnregisterIfAddressReused -d $imr_debug_level -o $imr_locator_ior -p $imr_db";
my $IMR_LOCATOR = new PerlACE::Process ("../../../ImplRepo_Service/ImplRepo_Service", $imr_locator_args);
my $TAO_IMR = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr");
my $svr_port_base = PerlACE::uniqueid () + 9000;
my $svr_a_id = "AAA";
my $svr_b_id = "BBB";
my $svr_a_ior = PerlACE::LocalFile ("A.ior");
my $svr_b_ior = PerlACE::LocalFile ("B.ior");
my $client_duration = 30;
my $svr_endpoint = "-ORBEndpoint " . "$protocol" . "://:" . "$svr_port_base/portspan=20";
my $imr_initref = "-ORBInitRef ImplRepoService=file://$imr_locator_ior";
my $svr_a_args = "$svr_endpoint $imr_initref -ORBServerId $svr_a_id -ORBUseIMR 1 -o $svr_a_ior";
my $svr_b_args = "$svr_endpoint $imr_initref -ORBServerId $svr_b_id -ORBUseIMR 1 -o $svr_b_ior";
my $cli_args = "$forward_opt -i file://$svr_a_ior -t $client_duration -e $got_object_not_exist_exception";

my $SVR_A = new PerlACE::Process ("serverA", $svr_a_args);
my $SVR_B = new PerlACE::Process ("serverB", $svr_b_args);
my $CLI = new PerlACE::Process ("client", $cli_args);

# Make sure the files are gone, so we can wait on them.
unlink $svr_a_ior;
unlink $svr_b_ior;
unlink $imr_db;
unlink $imr_locator_ior;

my $status = 0;
print STDERR $IMR_LOCATOR->CommandLine () . "\n";
$IMR_LOCATOR->Spawn ();
print STDERR "command " .$IMR_LOCATOR->CommandLine () . "\n";

if (PerlACE::waitforfile_timed ($imr_locator_ior, 10) == -1) {
    print STDERR "ERROR: cannot find $imr_locator_ior\n";
    $IMR_LOCATOR->Kill ();
    $status = 1;
}

sleep (2);
print STDERR "=== start server A: " . $SVR_A->CommandLine () . "\n";
$SVR_A->Spawn ();
print STDERR "command " . $SVR_A->CommandLine () . "\n";

if (PerlACE::waitforfile_timed ($svr_a_ior, 10) == -1) {
    print STDERR "ERROR: cannot find $svr_a_ior\n";
    $IMR_LOCATOR->Kill ();
    $SVR_A->Kill ();
    $status = 1;
}

print STDERR "=== start server B: " . $SVR_B->CommandLine () . "\n";
$SVR_B->Spawn ();
print STDERR "command " .$SVR_B->CommandLine () . "\n";

if (PerlACE::waitforfile_timed ($svr_b_ior, 10) == -1) {
    print STDERR "ERROR: cannot find $svr_b_ior\n";
    $IMR_LOCATOR->Kill ();
    $SVR_A->Kill ();
    $SVR_B->Kill ();
    $status = 1;
}

print STDERR "=== start client: " . $CLI->CommandLine () . "\n";
my $client = $CLI->Spawn ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $IMR_LOCATOR->Kill ();
    $SVR_A->Kill ();
    $SVR_B->Kill ();
    $status = 1;
}

sleep (5);

print STDERR "=== kill server A\n"; 
my $server = $SVR_A->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server A returned $server\n";
    $status = 1;
}

print STDERR "=== kill server B\n"; 
$server = $SVR_B->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server B returned $server\n";
    $status = 1;
}

sleep (5);
unlink $svr_a_ior;
unlink $svr_b_ior;

print STDERR "=== restart server B\n"; 
$svr_b_args = $svr_b_args . " -l $delay";
# Run -ORBDebugLevel 10 to see server raise OBJECT_NOT_EXIST exception.
$SVR_B = new PerlACE::Process ("serverB", $svr_b_args);
print STDERR $SVR_B->CommandLine () . "\n";

$SVR_B->Spawn ();
if (PerlACE::waitforfile_timed ($svr_b_ior, 10) == -1) {
    print STDERR "ERROR: cannot find $svr_b_ior\n";
    $IMR_LOCATOR->Kill ();
    $SVR_B->Kill ();
    $CLI->Kill ();
    $status = 1;
}

sleep ($delay * 2);

print STDERR "=== restart server A\n"; 
print STDERR $SVR_A->CommandLine () . "\n";
$SVR_A->Spawn ();
if (PerlACE::waitforfile_timed ($svr_a_ior, 10) == -1) {
    print STDERR "ERROR: cannot find $svr_a_ior\n";
    $IMR_LOCATOR->Kill ();
    $SVR_A->Kill ();
    $SVR_B->Kill ();
    $CLI->Kill ();
    $status = 1;
}

$CLI->WaitKill ($client_duration);

$IMR_LOCATOR->Kill();
$SVR_A->Kill();
$SVR_B->Kill();

# Make sure the files are gone, so we can wait on them.
unlink $svr_a_ior;
unlink $svr_b_ior;
unlink $imr_db;
unlink $imr_locator_ior;

exit $status;
