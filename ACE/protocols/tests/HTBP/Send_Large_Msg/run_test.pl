eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

$status = 0;

$host = hostname();
$port = PerlACE::random_port();
if (PerlACE::is_vxworks_test()) {
    $host =  $ENV{'ACE_RUN_VX_TGTHOST'};
    $SV = new PerlACE::ProcessVX ("server", "-p $port");
}
else {
    $SV = new PerlACE::Process ("server", "-p $port");
}

# The client code should later be modified to get the hostname
# using ACE_OS::hostname so the same script can be run on all
# hosts without having to reset the host where it has to be run.
$CL = new PerlACE::Process ("client", "-h $host -p $port");

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


exit $status;
