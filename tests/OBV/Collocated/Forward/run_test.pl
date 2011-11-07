eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $collocated = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$iorbase = "test.ior";
$iorfile = $collocated->LocalFile ("test.ior");
$collocated->DeleteFile ($iorbase);

$CO = $collocated->CreateProcess ("collocated");

$collocated_status = $CO->Spawn ();

if ($collocated_status != 0) {
    print STDERR "ERROR: server returned $collocated_status\n";
    exit 1;
}

if ($collocated->WaitForFileTimed ($iorbase,
                                   $collocated->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $CO->Kill (); $CO->TimedWait (1);
    exit 1;
}

$collocated_status = $CO->WaitKill ($collocated->ProcessStopWaitInterval ());

if ($collocated_status != 0) {
    print STDERR "ERROR: collocated returned $collocated_status\n";
    $collocated_status = 1;
}

$collocated->DeleteFile ($iorbase);

exit $collocated_status;
