eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } 
}

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);
my $host = new PerlACE::TestTarget;

$iorbase = "server.ior";
$iorfile = $target->LocalFile ("$iorbase");
$target->DeleteFile($iorfile);
$host->DeleteFile($iorbase);

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBDebuglevel $debug_level -o $iorbase");
}
else {
    $SV = $target->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $iorfile");
}
$CL = $host->CreateProcess ("client", "-k file://$iorbase");
    
$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if ($target->WaitForFileTimed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

if ($target->GetFile ($iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorfile>\n";
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

$target->GetStderrLog();

$target->DeleteFile($iorfile);
$host->DeleteFile ($iorbase);

exit $status;
