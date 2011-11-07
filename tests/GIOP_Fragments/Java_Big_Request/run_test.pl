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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $host = $server->HostName();
my $port = $server->RandomPort();
my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "");
## No ORB fragments GIOP 1.0 messages.
## The JDK ORB only fragments GIOP 1.2 messages.
foreach my $giop ('1.2') {
    print "Testing GIOP $giop Fragmentation\n";

    $server->DeleteFile($iorbase);
    $SV->Arguments("-ORBdebuglevel $debug_level -o $server_iorfile ".
                   "-ORBEndpoint iiop://$giop\@$host:$port");

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

    my $client_status = system('java client');
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        ++$status;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 10);

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        ++$status;
    }

    if ($status) {
        last;
    }
}

$server->DeleteFile($iorbase);

exit $status;
