eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

$iorfile = PerlACE::LocalFile ("server.ior");

unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("server");
$host = hostname();

# The client code should later be modified to get the hostname
# using ACE_OS::hostname so the same script can be run on all
# hosts without havng to reset the host where it has to be run.
$CL = new PerlACE::Process ("client", $host);

$SV->Spawn ();

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
