eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;

$port = 12345;

if (PerlACE::is_vxworks_test()) {
    $exe1 = new PerlACE::ProcessVX ("test", "-ORBUseSharedProfile 1 -o $iorfile -p $port");
    $exe2 = new PerlACE::ProcessVX ("test", "-ORBUseSharedProfile 1 -ORBSvcConf client.conf -k file://$iorfile");

}
else {
    $exe1 = new PerlACE::Process ("test", "-ORBUseSharedProfile 1 -o $iorfile -p $port");
    $exe2 = new PerlACE::Process ("test", "-ORBUseSharedProfile 1 -ORBSvcConf client.conf -k file://$iorfile");
}

print "Generating the IOR file\n";

$exe1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $exe1->Kill (); $exe1->TimedWait (1);
    exit 1;
}

# The server ought to die quickly on its own.
$server = $exe1->WaitKill (2);

if ($server != 0) {
    print STDERR "ERROR: First exe returned $server\n";
    $status = 1;
}

print "Processing the IOR file\n";

$server = $exe2->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);

if ($server != 0) {
    print STDERR "ERROR: Second exe returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
