eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $client_conf = $server->LocalFile ("mt_noupcall$PerlACE::svcconf_ext");
my $server_conf = $server->LocalFile ("mt_noupcall$PerlACE::svcconf_ext");

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-d') {
        $debug_level = '10';
    }
}

my $server_iorbase = "server.ior";
my $client_iorbase = "client.ior";
my $server_iorfile = $server->LocalFile ($server_iorbase);
my $client_iorfile = $client->LocalFile ($client_iorbase);

$server->DeleteFile($server_iorbase);
$client->DeleteFile($client_iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBDebugLevel $debug_level -ORBSvcConf $server_conf " .
                              "-o $server_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($server_iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL = $client->CreateProcess ("client", "-k file://$client1_iorfile");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->WaitForFileTimed ($client_iorbase,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "INFO: Awaiting server ...\n";

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 30);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

print "INFO: Awaiting client ...\n";

$client_status = $CL->WaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

print "INFO: Test succeeded\n";
exit 0;

