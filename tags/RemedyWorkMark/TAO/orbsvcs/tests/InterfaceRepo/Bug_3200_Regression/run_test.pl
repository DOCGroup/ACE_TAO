eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$nice = "";
$locking = "";

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $ior1file = "if_repo.ior";

#Files which used by server1
my $server_ior1file = $server->LocalFile ($ior1file);
$server->DeleteFile($ior1file);

#Files which used by client1
my $client1_ior1file = $client1->LocalFile ($ior1file);
$client1->DeleteFile($ior1file);

#Files which used by client2
my $client2_ior1file = $client2->LocalFile ($ior1file);
$client2->DeleteFile($ior1file);

$SV = $server->CreateProcess ("../../../IFR_Service/tao_ifr_service",
                              "-ORBdebuglevel $debug_level " .
                              "$nice " .
                              "-o $server_ior1file " .
                              "$locking");

$CL1 = $client1->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_ifr",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef InterfaceRepository=file://$client1_ior1file b1.idl");

$CL2 = $client2->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_ifr",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef InterfaceRepository=file://$client2_ior1file b2.idl");


print <<EOF;
              ********** IMPORTANT NOTE ***********
While it is easy to confirm this failure via inspection of
output from tao_ifr, at the time of committing this test tao_ifr
did not consistently exit with a non-zero return code when hitting
an error, making automatic detection of success/failure difficult.

If/when this behavior of tao_ifr changes, this test driver can be
updated so that it properly detects success/failure of tao_ifr.
******************************************************************
EOF

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($ior1file,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_ior1file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_ior1file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client1->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client1_ior1file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client2->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client2_ior1file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($ior1file);
$client1->DeleteFile($ior1file);
$client2->DeleteFile($ior1file);

exit $status;
