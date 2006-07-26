eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$dum_core = PerlACE::LocalFile ("core");
$status = 0;
unlink $dum_core;

$HELLO = new PerlACE::Process ("uipmc_test", "");

$prog = $HELLO->SpawnWaitKill (20);

if ($prog != 0) {
    print STDERR "ERROR: uipmc_test returned $prog\n";
    $status = 1;
}

if (-e $dum_core) {
    print STDERR "ERROR: uipmc_test core dumped\n";
    $status = 1;
}

unlink $dum_core;

exit $status;

