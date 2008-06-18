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
$synchbase = "ready";
$synchfile = PerlACE::LocalFile ("$synchbase");

print "port = $port\n";
if (PerlACE::is_vxworks_test()) {
    $host =  $ENV{'ACE_RUN_VX_TGTHOST'};
    $SV = new PerlACE::ProcessVX ("server", "-p $port -o $synchbase");
}
else {
    $SV = new PerlACE::Process ("server", "-p $port -o $synchfile");
}

# The client code should later be modified to get the hostname
# using ACE_OS::hostname so the same script can be run on all
# hosts without havng to reset the host where it has to be run.
$CL = new PerlACE::Process ("client", " -h $host -p $port");

unlink $synchfile;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($synchfile,
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$synchfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

unlink $synchfile;

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
