eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::stat;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sv1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sv2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $cl = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

# amount of delay between running the servers
$sleeptime = 6;

$nsiorfile = "ns.ior";
$test1file = "test1";
$test2file = "test2";
$inputfile = "test_input";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv1_nsiorfile = $sv1->LocalFile ($nsiorfile);
my $sv2_nsiorfile = $sv2->LocalFile ($nsiorfile);
my $cl_nsiorfile = $cl->LocalFile ($nsiorfile);
my $sv1_test1file = $sv1->LocalFile ($test1file);
my $sv2_test2file = $sv2->LocalFile ($test2file);
$ns->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$sv2->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cl->DeleteFile ($inputfile);
$sv1->DeleteFile ($test1file);
$sv2->DeleteFile ($test2file);

# generate test stream data
$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $cl_inputfile = $cl->LocalFile ($inputfile);
if ($cl->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_inputfile>\n";
    exit 1;
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");
$SV1 = $sv1->CreateProcess ("server",
                            " -ORBDebugLevel $debug_level ".
                            " -ORBInitRef NameService=file://$sv1_nsiorfile ".
                            "-f $sv1_test1file");
$SV2 = $sv2->CreateProcess ("server",
                            " -ORBDebugLevel $debug_level ".
                            " -ORBInitRef NameService=file://$sv2_nsiorfile ".
                            "-f $sv2_test2file");
$CL = $cl->CreateProcess ("ftp",
                          " -ORBInitRef NameService=file://$cl_nsiorfile ".
                          "-f $cl_inputfile");

print STDERR "Starting Naming Service\n";
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sv1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sv2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cl->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Server 1\n";
$SV1_status = $SV1->Spawn ();
if ($SV1_status != 0) {
    print STDERR "ERROR: Server 1 returned $SV1_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Server 2\n";
$SV2_status = $SV2->Spawn ();
if ($SV2_status != 0) {
    print STDERR "ERROR: Server 2 returned $SV2_status\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Client\n";
$CL_status = $CL->SpawnWaitKill ($cl->ProcessStartWaitInterval()+45);
if ($CL_status != 0) {
    print STDERR "ERROR: client returned $CL_status\n";
    $status = 1;
}

$SV1_status = $SV1->TerminateWaitKill ($sv1->ProcessStopWaitInterval());
if ($SV1_status != 0) {
    print STDERR "ERROR: server 1 returned $SV1_status\n";
    $status = 1;
}

$SV2_status = $SV2->TerminateWaitKill ($sv2->ProcessStopWaitInterval());
if ($SV2_status != 0) {
    print STDERR "ERROR: server 2 returned $SV2_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$sv2->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cl->DeleteFile ($inputfile);
$sv1->DeleteFile ($test1file);
$sv2->DeleteFile ($test2file);

exit $status;
