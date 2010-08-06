eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
@files= <core.*>;
unlink $iorfile;
unlink (@files);
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}

$iterations = 1000;

if ($ARGV[0] eq '-quick')  {
    $iterations = 100;
}

$CL = new PerlACE::Process ("client", " -k file://$iorfile -i $iterations");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL->Spawn (60);

$client = $CL->WaitKill (960);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

@files= <core.*>;
#unlink (@files);
unlink $iorfile;

exit $status;
