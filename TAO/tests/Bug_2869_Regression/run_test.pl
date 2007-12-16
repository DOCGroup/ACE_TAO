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

my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
$CL = new $class ("client");

$client = $CL->Spawn ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$client = $CL->WaitKill (15);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
