eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a perl script that runs the NamingContextExt test. It starts 
# the Naming service, server and the client as necessary

use lib '../../../../bin';
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client 
# to allow proper server initialization.
$sleeptime = 8;

# Variables for command-line arguments to client and server
# executables
$iorfile = PerlACE::LocalFile ("ns.ior");

# Run the server and client for the test. 

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service", "-o $iorfile");
$CL = new PerlACE::Process ("client", "-s -ORBInitRef NameService=file://$iorfile");

# Make sure the files are gone, so we can wait on them.
unlink $iorfile;

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
    print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
    $NS->Kill ();
    exit 1;
}
   
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: server returned $nserver\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
