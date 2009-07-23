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
$ciao_root = "$ENV{CIAO_ROOT}";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    } 
}

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);
my $host = new PerlACE::TestTarget;

$iorbase1 = "server1.ior";
$iorbase2 = "server2.ior";
$ior1 = $target->LocalFile ("$iorbase");
$ior2 = $target->LocalFile ("$iorbase");
$target->DeleteFile($iorfile1);
$target->DeleteFile($iorfile2);
$host->DeleteFile($iorbase1);
$host->DeleteFile($iorbase2);

if (PerlACE::is_vxworks_test()) {
    $SV1 = new PerlACE::ProcessVX ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $iorbase1 -o $iorbase1");
    $SV2 = new PerlACE::ProcessVX ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $iorbase2 -o $iorbase2");
}
else {
    $SV1 = $target->CreateProcess ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $iorbase1 -o $iorbase1");
    $SV2 = $target->CreateProcess ("$ciao_root/bin/ciao_componentserver", "-ORBDebuglevel $debug_level -u $iorbase2 -o $iorbase2");
}
$CL = $host->CreateProcess ("client", "-k file://$iorbase1 -j file://$iorbase2");
    
$server1 = $SV1->Spawn ();
$server2 = $SV2->Spawn ();

if ($server1 != 0) {
    print STDERR "ERROR: server1 returned $server\n";
    exit 1;
}
if ($server2 != 0) {
    print STDERR "ERROR: server2 returned $server\n";
    exit 1;
}

if ($target->WaitForFileTimed ($iorbase1,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 
if ($target->WaitForFileTimed ($iorbase2,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

if ($target->GetFile ($iorfile1, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorfile1>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}if ($target->GetFile ($iorfile2, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorfile2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (10);
$server = $SV2->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$target->GetStderrLog();

$target->DeleteFile($iorfile);
$host->DeleteFile ($iorbase);

exit $status;
