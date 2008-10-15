eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

# Usually the primary component to run on targets (if any) is the server;
# this time it's the client.
$client = PerlACE::TestTarget::create_target(1) || die "Create target failed\n";

$iorbase = "server_on_localhost_1192.ior";
$iorfile = PerlACE::LocalFile ($iorbase);
$client_iorfile = $client->LocalFile($iorbase);
$status = 0;

## Get the perl interpreter that invoked us and remove any
## executable extension (if there is one).
my($perl) = $^X;
$perl =~ s/\.exe$//i;
$perl =~ s/000000\///g if ($^O eq 'VMS');

$SV = new PerlACE::Process ($perl, "fakeserver2.pl");
if (PerlACE::is_vxworks_test()) {
$CL = new PerlACE::ProcessVX ("client", " -k file://$iorbase -ORBdebuglevel 1 -ORBlogfile client.log");
}
else {
$CL = $client->CreateProcess ("client", " -k file://$client_iorfile -ORBdebuglevel 1 -ORBlogfile client.log");
}
$client->DeleteFile("client.log");

$SV->IgnoreExeSubDir(1);
$SV->IgnoreHostRoot(1);
$SV->Spawn ();
sleep(1);  # give the server a chance to come up

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorfile, $client_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client_status = $CL->SpawnWaitKill (60);

# We expect to have to kill both client and server.

#if ($client != 0) {
#    print STDERR "ERROR: client returned $client\n";
#    $status = 1;
#}

$server = $SV->WaitKill (10);

#if ($server != 0) {
#    print STDERR "ERROR: server returned $server\n";
#    $status = 1;
#}

open (LOG, "client.log") or die "Couldn't open client log file client.log: $!\n";
while (<LOG>) {
  $ccmsgfound = 1 if (/process_parsed_messages, received CloseConnection message/);
}
close (LOG);
if (not $ccmsgfound) {
  print STDERR "ERROR: didn't find CloseConnection debug message in client log.\n";
  $status = 1;
}

exit $status;
