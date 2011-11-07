eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$port = PerlACE::uniqueid () + 12000;
$endien = (pack('L', 0x41424344) eq 'ABCD' ? '_be' : '');

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $hostname = $server->HostName();
my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBEndpoint iiop://$hostname:$port " .
                              "-ORBDebugLevel $debug_level " .
                              "-o $server_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $server->DeleteFile($iorbase);
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $server->DeleteFile($iorbase);
    exit 1;
}

my($CL) = system("$^X dribble.pl --host=$hostname --port=$port " .
                 "--stream=giop1.2_fragments$endien.dat " .
                 "--layout=giop1.2_fragments$endien.layout");
if ($CL != 0) {
    print STDERR "ERROR: client returned $CL\n";
    $status = 2;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 3;
}

$server->DeleteFile($iorbase);

exit $status;
