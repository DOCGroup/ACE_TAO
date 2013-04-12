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

my $server = PerlACE::TestTarget::create_target (1) || die "Create failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create failed\n";

my $TARGETHOSTNAME = $server2->HostName ();
my $port = $server2->RandomPort ();

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $server2_iorfile = $server2->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$server2->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$SV2 = $server2->CreateProcess ("server2",
                                "-k file://$server2_iorfile ".
                                "-ORBListenEndpoints iiop://$TARGETHOSTNAME:$port ".
                                "-a corbaloc:iiop:$TARGETHOSTNAME:$port/SIMPLE_TEST_KEY");

print "Starting Server\n";
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server2->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "Starting Other Server\n";
$server2_status = $SV2->Spawn ();

if ($server2_status != 0) {
    print STDERR "ERROR: server2 returned $server2_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server2_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server2_status != 0) {
    print STDERR "ERROR: server2 returned $server2_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$server2->DeleteFile($iorbase);

exit $status;
