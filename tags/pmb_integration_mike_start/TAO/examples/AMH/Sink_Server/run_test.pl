eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs an AMH Sink Server.
# See README file for details about the demo.

use lib "../../../../bin";
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client.
$sleeptime = 8;

# Amount of delay (in milliseconds) that the sink-server delays a client reply.
$reply_delay_time = 11;

# Number of calls a client makes to the AMH server
$iterations = 100;

# File used to pass AMH server ior to its clients.
$iorfile = PerlACE::LocalFile("amh.ior");

unlink $iorfile;

$delay_time = $reply_delay_time*1000; #convert to microseconds
$AMH = new PerlACE::Process ("st_server", "-ORBsvcconf st_server" .
                             "$PerlACE::svcconf_ext".
                             " -o $iorfile -s $delay_time");
$CL = new PerlACE::Process ("client", "-k file://$iorfile -n $iterations");

print STDERR "\n    Starting AMH Sink Server with $reply_delay_time milliseconds delayed response: \n\n";

# Run the AMH server.
$AMH->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
    print STDERR "ERROR: File containing AMH Server ior,".
        " <$iorfile>, cannot be found\n";
    $AMH->Kill ();
    exit 1;
}


# Run client.
print STDERR "\n     Client making $iterations calls to server: \n";
$evt_per_sec = 1000/$reply_delay_time;
print STDERR "\n(Expect less than $evt_per_sec Events/sec) \n\n";
$client = $CL->SpawnWaitKill (300);

# Clean up.
$amhserver= $AMH->TerminateWaitKill (5);

if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
