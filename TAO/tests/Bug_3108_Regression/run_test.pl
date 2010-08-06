eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$debug_opts_sv = '';
$debug_opts_cl = '';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $debug_opts_sv = '-ORBLogFile server.log';
        $debug_opts_cl = '-ORBLogFile client.log';
    }
}

my $target1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $target2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$iorbase = "server.ior";
$iorfile1 = $target1->LocalFile ("$iorbase");
$iorfile2 = $target2->LocalFile ("$iorbase");
$target1->DeleteFile($iorbase);
$target2->DeleteFile($iorbase);

$SV = $target1->CreateProcess ("server", "$debug_opts_sv -ORBdebuglevel $debug_level -o $iorfile1");
$CL = $target2->CreateProcess ("client", "$debug_opts_cl -ORBDebugLevel $debug_level -k file://$iorfile2");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if ($target1->WaitForFileTimed ($iorbase,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$target1->GetStderrLog();
$target2->GetStderrLog();

$target1->DeleteFile($iorbase);
$target2->DeleteFile($iorbase);

exit $status;
