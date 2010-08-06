eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$ORBdebuglevel = 0;
$status = 0;

$iorfile = "receiver.ior";
unlink $iorfile;

$SV = new PerlACE::Process ("receiver", "-ORBdebuglevel $ORBdebuglevel");
$CL = new PerlACE::Process ("sender", "-ORBdebuglevel $ORBdebuglevel -a 0 -b 0 -c 0 -d 0 -e 0 -i 10 -k file://receiver.ior -m 0 -p IIOP -r 5 -s 100 -t 0 -x 1");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$sender = $CL->SpawnWaitKill (60);
$receiver = $SV->WaitKill (5);

unlink $iorfile;

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

exit $status;
