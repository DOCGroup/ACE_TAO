eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("ec.ior");

unlink $iorfile;

$T = new PerlACE::Process ("Service",  "-o $iorfile");
$C = new PerlACE::Process ("Consumer", "file://$iorfile");
$S = new PerlACE::Process ("Supplier", "file://$iorfile");

$T->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $T->Kill (); 
    exit 1;
}

$C->Spawn ();

sleep 5;

$supplier = $S->SpawnWaitKill (120);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$consumer = $C->WaitKill (15);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$service = $T->TerminateWaitKill (5);

if ($service != 0) {
    print STDERR "ERROR: service returned $service\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
