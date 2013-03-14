eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

# number of threads to use for multithreaded clients or servers

$num_threads = 4;

# amount of delay between running the servers

$sleeptime = 15;

# other variables

$n = 1;
$debug = "";
$cm = "";
$sm = "";

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-n num] [-d level] [-h] [-nt] [-cm] [-sm]\n";
        print "\n";
        print "-n num              -- runs the client num times\n";
        print "-d level            -- runs each at debug level <level>\n";
        print "-h                  -- prints this information\n";
        print "-nt num             -- number of threads in the client (twice for server)\n";
        print "                       make sure this is before any -cm or -sm\n";
        print "-cm                 -- use more than one thread in the client\n";
        print "-sm                 -- use more than one thread in the server\n";
        print "-debug              -- runs server with -ORBDebugLevel 10\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-n") {
      $n = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-nt") {
      $num_threads = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-debug") {
      $debug = $debug." -d $ARGV[$i + 1]";
      $i++;
    }
    elsif ($ARGV[$i] eq "-cm") {
      $cm = "-n ".$num_threads;
    }
    elsif ($ARGV[$i] eq "-sm") {
      $sm = "-n ".($num_threads * 3);
    }
    elsif ($ARGV[$i] eq "-debug") {
      $debug_level = '10';
    }
}

my $nstarget = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $lctarget = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $svtarget = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $fftarget = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $gftarget = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $cltarget = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

my $nsiorbase = "ns.ior";
my $nstarget_nsiorfile = $nstarget->LocalFile ($nsiorbase);
my $lctarget_nsiorfile = $lctarget->LocalFile ($nsiorbase);
my $svtarget_nsiorfile = $svtarget->LocalFile ($nsiorbase);
my $fftarget_nsiorfile = $fftarget->LocalFile ($nsiorbase);
my $gftarget_nsiorfile = $gftarget->LocalFile ($nsiorbase);
my $cltarget_nsiorfile = $cltarget->LocalFile ($nsiorbase);
$nstarget->DeleteFile($nsiorbase);
$lctarget->DeleteFile($nsiorbase);
$svtarget->DeleteFile($nsiorbase);
$fftarget->DeleteFile($nsiorbase);
$gftarget->DeleteFile($nsiorbase);
$cltarget->DeleteFile($nsiorbase);

# Programs that are run

$NS = $nstarget->CreateProcess (
    "../../orbsvcs/Naming_Service/tao_cosnaming",
    "-o $nstarget_nsiorfile");
$LC = $lctarget->CreateProcess (
    "../../orbsvcs/LifeCycle_Service/tao_coslifecycle",
    "$debug -ORBInitRef NameService=file://$lctarget_nsiorfile");

$SV = $svtarget->CreateProcess (
    "server",
    "$debug $sm -ORBDebugLevel $debug_level ".
    "-ORBInitRef NameService=file://$svtarget_nsiorfile");
$FF = $fftarget->CreateProcess (
    "Factory_Finder",
    "$debug -ORBInitRef NameService=file://$fftarget_nsiorfile");
$GF = $gftarget->CreateProcess (
    "Generic_Factory",
    "-l $debug -ORBInitRef NameService=file://$gftarget_nsiorfile");
$CL = $cltarget->CreateProcess (
    "client",
    "-l $debug $cm -ORBInitRef NameService=file://$cltarget_nsiorfile");

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
if ($lctarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$lctarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($svtarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$svtarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($fftarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$fftarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($gftarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$gftarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cltarget->PutFile ($nsiorbase) == -1) {
    print STDERR "ERROR: cannot set file <$cltarget_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$lc_status = $LC->Spawn ();

if ($lc_status != 0) {
    print STDERR "ERROR: tao_coslifecycle returned $lc_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

$sv_status = $SV->Spawn ();

if ($sv_status != 0) {
    print STDERR "ERROR: server returned $sv_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $LC->Kill (); $LC->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

$ff_status = $FF->Spawn ();

if ($ff_status != 0) {
    print STDERR "ERROR: Factory_Finder returned $ff_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $LC->Kill (); $LC->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

$gf_status = $GF->Spawn ();

if ($gf_status != 0) {
    print STDERR "ERROR: Generic_Factory returned $gf_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $LC->Kill (); $LC->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    $FF->Kill (); $FF->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

for ($j = 1; $j <= $n; ++$j) {
    $cl_status =
        $CL->SpawnWaitKill ($cltarget->ProcessStartWaitInterval() + 45);

    if ($cl_status != 0) {
        print STDERR "ERROR: client returned $cl_status\n";
        $NS->Kill (); $NS->TimedWait (1);
        $LC->Kill (); $LC->TimedWait (1);
        $SV->Kill (); $SV->TimedWait (1);
        $FF->Kill (); $FF->TimedWait (1);
        $GF->Kill (); $GF->TimedWait (1);
        exit 1;
    }
}

$gf_status = $GF->TerminateWaitKill ($gftarget->ProcessStopWaitInterval());

if ($gf_status != 0) {
    print STDERR "ERROR: Generic_Factory returned $gf_status\n";
    $status = 1;
}

$ff_status = $FF->TerminateWaitKill ($fftarget->ProcessStopWaitInterval());

if ($ff_status != 0) {
    print STDERR "ERROR: Factory_Finder returned $ff_status\n";
    $status = 1;
}

$sv_status = $SV->TerminateWaitKill ($svtarget->ProcessStopWaitInterval());

if ($sv_status != 0) {
    print STDERR "ERROR: server returned $sv_status\n";
    $status = 1;
}

$lc_status = $LC->TerminateWaitKill ($lctarget->ProcessStopWaitInterval());

if ($lc_status != 0) {
    print STDERR "ERROR: tao_coslifecycle returned $lc_status\n";
    $status = 1;
}

$ns_status = $NS->TerminateWaitKill ($nstarget->ProcessStopWaitInterval());

if ($ns_status != 0) {
    print STDERR "ERROR: tao_cosnaming returned $ns_status\n";
    $status = 1;
}

$nstarget->DeleteFile($nsiorbase);
$lctarget->DeleteFile($nsiorbase);
$svtarget->DeleteFile($nsiorbase);
$fftarget->DeleteFile($nsiorbase);
$gftarget->DeleteFile($nsiorbase);
$cltarget->DeleteFile($nsiorbase);

exit $status;
