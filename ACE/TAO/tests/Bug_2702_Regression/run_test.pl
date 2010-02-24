eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

# Usually the primary component to run on targets (if any) is the server;
# this time it's the client.
my $client = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $server = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

my $iorbase = "server_on_localhost_1192.ior";
my $logfile = "client.log";

my $client_iorfile = $client->LocalFile($iorbase);
my $client_logfile = $client->LocalFile($logfile);
my $server_logfile = $server->LocalFile($logfile);
$client->DeleteFile($logfile);

## Get the perl interpreter that invoked us and remove any
## executable extension (if there is one).
my($perl) = $^X;
$perl =~ s/\.exe$//i;
$perl =~ s/000000\///g if ($^O eq 'VMS');

$SV = $server->CreateProcess ($perl, "fakeserver2.pl");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile ".
                                        "-ORBdebuglevel 1 ".
                                        "-ORBlogfile $client_logfile");

$SV->IgnoreExeSubDir(1);
$SV->IgnoreHostRoot(1);

$server_status = $SV->Spawn ();

sleep(1);  # give the server a chance to come up

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorbase>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

open (LOG, $server_logfile) or die "Couldn't open server log file $server_logfile: $!\n";
while (<LOG>) {
    $ccmsgfound = 1 if (/process_parsed_messages, received CloseConnection message/);
}
close (LOG);
if (not $ccmsgfound) {
    print STDERR "ERROR: didn't find CloseConnection debug message in client log.\n";
    $status = 1;
}

exit $status;
