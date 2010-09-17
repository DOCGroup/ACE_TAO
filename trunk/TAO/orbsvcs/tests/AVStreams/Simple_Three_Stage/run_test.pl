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
my $sv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $re = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $di = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

# amount of delay between running the servers
$sleeptime = 10;

$nsiorfile = "ns.ior";
$outputfile = "output";
$inputfile = "test_input";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv_nsiorfile = $sv->LocalFile ($nsiorfile);
my $re_nsiorfile = $re->LocalFile ($nsiorfile);
my $di_nsiorfile = $di->LocalFile ($nsiorfile);
my $re_outputfile = $re->LocalFile ($outputfile);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$re->DeleteFile ($nsiorfile);
$di->DeleteFile ($nsiorfile);
$sv->DeleteFile ($inputfile);
$re->DeleteFile ($outputfile);

# generate test stream data
$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $sv_inputfile = $sv->LocalFile ($inputfile);
if ($sv->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_inputfile>\n";
    exit 1;
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");
$SV = $sv->CreateProcess ("sender",
                          " -ORBDebugLevel $debug_level ".
                          " -ORBInitRef NameService=file://$sv_nsiorfile ".
                          "-f $sv_inputfile");
$RE = $re->CreateProcess ("receiver",
                          " -ORBInitRef NameService=file://$re_nsiorfile ".
                          "-f $re_outputfile");
$DI = $di->CreateProcess ("distributer",
                          " -ORBInitRef NameService=file://$di_nsiorfile ");

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
if ($sv->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Receiver\n";
$RE_status = $RE->Spawn ();
if ($RE_status != 0) {
    print STDERR "ERROR: Receiver returned $RE_status\n";
    $RE->Kill (); $RE->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Sender\n";
$SV_status = $SV->Spawn ();
if ($SV_status != 0) {
    print STDERR "ERROR: Sender returned $SV_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $RE->Kill (); $RE->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Distributer\n";
$DI_status = $DI->SpawnWaitKill ($di->ProcessStartWaitInterval()+185);
if ($DI_status != 0) {
    print STDERR "ERROR: Distributer returned $DI_status\n";
    $status = 1;
}

$SV_status = $SV->TerminateWaitKill ($sv->ProcessStopWaitInterval()+45);
if ($SV_status != 0) {
    print STDERR "ERROR: sender returned $SV_status\n";
    $status = 1;
}

$RE_status = $RE->TerminateWaitKill ($re->ProcessStopWaitInterval()+45);
if ($RE_status != 0) {
    print STDERR "ERROR: receiver returned $RE_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval()+45);
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$re->DeleteFile ($nsiorfile);
$di->DeleteFile ($nsiorfile);
$sv->DeleteFile ($inputfile);
$re->DeleteFile ($outputfile);

exit $status;
