eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$HELLO = new PerlACE::Process ("uipmc_test",
    "-u corbaloc:miop:1.0\@1.0-cdmwftdomain-1/225.1.1.8:5555 " .
    "-c corbaloc:miop:1.0\@1.0-cdmwftdomain-1/225.1.1.7:5555");

$prog = $HELLO->SpawnWaitKill (20);

if ($prog != 0) {
    print STDERR "ERROR: uipmc_test returned $prog\n";
    $status = 1;
}

exit $status;
