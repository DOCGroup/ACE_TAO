#!/usr/bin/perl
use strict;
use warnings;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "server target";
my $client = PerlACE::TestTarget::create_target (2) || die "client target";
my $port = $server->RandomPort ();
my $host = $server->HostName ();
my $ready = $server->LocalFile ('ready');
my $debug = @ARGV ? 10 : 0;
$server->DeleteFile ('ready');
my $sv = $server->CreateProcess ('server',
  "-ORBSvcConf server.conf -ORBEndpoint iiop://$host:$port -ORBDebugLevel $debug -o $ready");
my $cl = $client->CreateProcess ('client', "$host:$port");
if ($sv->Spawn () != 0) { exit 1; }
if ($server->WaitForFileTimed ('ready', $server->ProcessStartWaitInterval ()) == -1) {
  $sv->Kill (); $sv->TimedWait (1); exit 1;
}
my $status = $cl->SpawnWaitKill ($client->ProcessStartWaitInterval () + 20);
# Let the bounded server run finish, exercising scanner cancellation at destroy.
my $server_status = $sv->WaitKill ($server->ProcessStopWaitInterval () + 40);
$server->DeleteFile ('ready');
exit (($status == 0 && $server_status == 0) ? 0 : 1);
