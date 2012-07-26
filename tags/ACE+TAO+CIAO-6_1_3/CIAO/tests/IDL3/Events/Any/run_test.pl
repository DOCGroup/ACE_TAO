eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Do not use environment variables here since not all platforms use ACE_ROOT
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use English;

$status = 0;
$exflags = "";

# Run the test

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$EX = $tg->CreateProcess ("EventAny");
$EX->Arguments ($exflags);
$test = $EX->SpawnWaitKill ($tg->ProcessStopWaitInterval ());

if ($test != 0) {
    print STDERR "ERROR: test error status $test\n";
    $status = 1;
}

exit $status;
