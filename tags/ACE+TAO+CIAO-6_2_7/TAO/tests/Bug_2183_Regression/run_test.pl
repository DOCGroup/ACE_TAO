eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;


my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


# The server IOR file
$iorbase = "server.ior";
$server_ior_file = $server->LocalFile ($iorbase);

# The server configuration file
$server_conf_file = "server.conf";
$server_conf = $server->LocalFile ($server_conf_file);

$server->DeleteFile ($iorbase);

# The client and server processes
$perl_executable = $^X;
$perl_executable =~ s/\.exe//ig;
if ($^O == 'VMS') {
    $perl_executable =~ s/000000\///g;
}

$port = '15000';

# copy the configuation file
if ($server->PutFile ($server_conf_file) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}

$SV = $server->CreateProcess ("server",
                              "-o $server_ior_file " .
                              "-ORBEndpoint iiop://:$port " .
                              "-ORBSvcConf $server_conf");

$CL1 = $client->CreateProcess ("client", "-k file://$server_ior_file");
$CL2 = $client->CreateProcess ("$perl_executable", "hang_client.pl ".
                                                   $server->{'HOSTNAME'});

$CL2->IgnoreHostRoot(1);
$CL2->IgnoreExeSubDir(1);


# Fire up the server
$status_server = $SV->Spawn();

if ($status_server != 0) {
   print STDERR "ERROR: server returned $status_server\n";
   exit 1;
}

# We can wait on the IOR file
if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_ior_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL2->Spawn ();

$CL2->Wait ($server->ProcessStartWaitInterval());

$client_status = $CL1->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: Bug #2183 Regression failed. Client returned $client_status\n";
    $status = 1;
    $SV->Kill ();
    $CL2->Kill ();
    $server->DeleteFile ($iorbase);
    exit $status;
}

print "Test succeeded !!!\n";

# Clean up and return
$SV->TerminateWaitKill ($server->ProcessStopWaitInterval());
$CL2->TerminateWaitKill ($server->ProcessStopWaitInterval());

$server->DeleteFile ($iorbase);

exit 0;
