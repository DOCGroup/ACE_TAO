eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $CO = new PerlACE::ProcessVX ("collocated");
}
else {
    $CO = new PerlACE::Process ("collocated");
}

$server_status = $CO->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$result = $CO->WaitKill (10);

if ($result != 0) {
    print STDERR "ERROR: collocated returned $result\n";
    $result = 1;
}

unlink $iorfile;

exit $status;
