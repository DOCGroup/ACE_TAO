eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$transients = 0;
$debug_level = 0;
$cdebug_level = 0;
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = 10;
    }
    if ($i eq '-cdebug') {
      $cdebug_level = 3;
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $port = $server->RandomPort ();
my $width = 3;

my $server_arg = "-ORBListenEndpoints iiop://127.0.0.1:$port -o $server_iorfile";
$server_arg .= " -ORBdebuglevel $debug_level" if ($debug_level > 0);

my $client_arg = "-ORBIIOPClientPortBase $port -ORBIIOPClientPortSpan $width"
                 . " -k file://$client_iorfile";
$client_arg .= " -ORBdebuglevel $cdebug_level" if ($cdebug_level > 0);

$SV = $server->CreateProcess ("server", $server_arg);
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
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

for ($i = 0; $i <= $width; $i++) {
    $args = $client_arg;
    $args .= " -ORBLogFile clt$i" if ($cdebug_level > 0);

    $CL[$i] = $client->CreateProcess ("client", $args);
    $CL[$i]->Spawn ();
}

for ($i = 0; $i <= $width; $i++) {
    $client_status = $CL[$i]->WaitKill ($client->ProcessStopWaitInterval());

    if ($client_status == 100) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
    elsif ($client_status == 1) {
        print STDERR "client $i caught expected transient\n";
        $transient++;
    }
}

if ($transient == 0) {
    $status = 2;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
