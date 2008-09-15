eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");

unlink $file;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("Collocated_Test", "-ORBObjRefStyle url");
}
else {
    $SV = new PerlACE::Process ("Collocated_Test", "-ORBObjRefStyle url");
}

print STDERR "\n\n==== Running interceptor collocated Dynamic test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

$collocated = $SV->WaitKill (5);

if ($collocated != 0) {
    print STDERR "ERROR: Collocated_Test returned $collocated\n";
    $status = 1;
}

unlink $file;

exit $status;
