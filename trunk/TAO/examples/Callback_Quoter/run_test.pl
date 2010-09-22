eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

# amount of delay between running the servers

$sleeptime = 15;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $nstarget = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ntarget = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ctarget = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $starget = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $nsiorbase = "ns.ior";
my $examplebase = "example.stocks";
my $nstarget_nsiorfile = $nstarget->LocalFile ($nsiorbase);
my $ntarget_nsiorfile = $ntarget->LocalFile ($nsiorbase);
my $ctarget_nsiorfile = $ctarget->LocalFile ($nsiorbase);
my $starget_nsiorfile = $starget->LocalFile ($nsiorbase);
my $starget_examplefile = $starget->LocalFile ($examplebase);
$nstarget->DeleteFile($nsiorbase);
$ntarget->DeleteFile($nsiorbase);
$ctarget->DeleteFile($nsiorbase);
$starget->DeleteFile($nsiorbase);

# Programs that are run

$NS = $nstarget->CreateProcess (
    "../../orbsvcs/Naming_Service/tao_cosnaming",
    "-o $nstarget_nsiorfile");
$N = $ntarget->CreateProcess (
    "notifier",
    "-ORBDebugLevel $debug_level ".
    "-ORBInitRef NameService=file://$ntarget_nsiorfile");
$C = $ctarget->CreateProcess (
    "consumer",
    "-ORBInitRef NameService=file://$ctarget_nsiorfile -t 12 -a TAO");
$S = $starget->CreateProcess (
    "supplier",
    "-ORBInitRef NameService=file://$starget_nsiorfile -i $starget_examplefile");

print STDERR "================ Remote test\n";

$ns_status = $NS->Spawn ();

if ($ns_status != 0) {
    print STDERR "ERROR: tao_cosnaming returned $ns_status\n";
    exit 1;
}

if ($nstarget->WaitForFileTimed ($nsiorbase,
                                 $nstarget->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nstarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($nstarget->GetFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nstarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ntarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$ntarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ctarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$ctarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($starget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$starget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$n_status = $N->Spawn ();

if ($n_status != 0) {
    print STDERR "ERROR: notifier returned $n_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

$c_status = $C->Spawn ();

if ($c_status != 0) {
    print STDERR "ERROR: consumer returned $c_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $N->Kill (); $N->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

$s_status = $S->SpawnWaitKill ($starget->ProcessStopWaitInterval() + 60);

if ($s_status != 0) {
    print STDERR "ERROR: supplier returned $s_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $N->Kill (); $N->TimedWait (1);
    $C->Kill (); $C->TimedWait (1);
    exit 1;
}

$c_status = $C->TerminateWaitKill ($ctarget->ProcessStopWaitInterval());

if ($c_status != 0) {
    print STDERR "ERROR: consumer returned $c_status\n";
    $status = 1;
}

$n_status = $N->TerminateWaitKill ($ntarget->ProcessStopWaitInterval());

if ($n_status != 0) {
    print STDERR "ERROR: notifier returned $n_status\n";
    $status = 1;
}

$ns_status = $NS->TerminateWaitKill ($nstarget->ProcessStopWaitInterval());

if ($ns_status != 0) {
    print STDERR "ERROR: tao_cosnaming returned $ns_status\n";
    $status = 1;
}

$nstarget->DeleteFile($nsiorbase);
$ntarget->DeleteFile($nsiorbase);
$ctarget->DeleteFile($nsiorbase);
$starget->DeleteFile($nsiorbase);

exit $status;
