eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $es = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $con = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $sup = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $test = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

$nsiorfile = "NameService.ior";
$ecconffile = "ec$PerlACE::svcconf_ext";
$ecmtconffile = "ec$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $es_nsiorfile = $es->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $test_ecconffile = $test->LocalFile ($ecconffile);
my $es_ecconffile = $es->LocalFile ($ecconffile);
my $test_ecmtconffile = $test->LocalFile ($ecmtconffile);
$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);

print STDERR "================ Collocated tests, single threaded\n";

$T = $test->CreateProcess ("ECT_Throughput",
                           "-ORBSvcConf $test_ecconffile ".
                           "-u 10000 -n 1 -t 0 -c 4");

$T_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval()+105);
if ($T_status != 0) {
    print STDERR "ERROR: test returned $T_status\n";
    $status = 1;
}

print STDERR "================ Collocated tests, multi threaded\n";

$T = $test->CreateProcess ("ECT_Throughput",
                           "-ORBSvcConf $test_ecmtconffile ".
                           "-u 10000 -n 1 -t 0 -c 4");

$T_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval()+105);
if ($T_status != 0) {
    print STDERR "ERROR: test returned $T_status\n";
    $status = 1;
}

print STDERR "================ Remote test\n";

$NS = $ns->CreateProcess ("../../Naming_Service/Naming_Service",
                           "-o $ns_nsiorfile");

$ES = $es->CreateProcess ("../../Event_Service/Event_Service",
                          "-ORBInitRef NameService=file://$es_nsiorfile ".
                          "-ORBSvcConf $es_ecconffile ".
                          "-s local");

$CON = $con->CreateProcess ("ECT_Consumer",
                            "-ORBDebugLevel $debug_level ".
                            "-ORBInitRef NameService=file://$con_nsiorfile ".
                            "-c 4 -s 1");

$SUP = $sup->CreateProcess ("ECT_Supplier",
                            "-ORBInitRef NameService=file://$sup_nsiorfile ".
                            " -s 1 -u 5000 -n 1 -t 0");

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($es->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$es_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$ES_status = $ES->Spawn ();
if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    $ES->Kill (); $ES->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

sleep 10;

$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $ES->Kill (); $ES->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $CON->Kill (); $CON->TimedWait (1);
    $ES->Kill (); $ES->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval()+285);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status = 1;
}

$ES_status = $ES->TerminateWaitKill ($es->ProcessStopWaitInterval());
if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);

exit $status;
