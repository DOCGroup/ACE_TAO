eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print STDERR "\n\n==== Running Bug_2137_Regression (FT ::is_equivalent) test\n";

if (PerlACE::is_vxworks_test())
{
    $T = new PerlACE::ProcessVX ("client");
}
else
{
    $T = new PerlACE::Process ("client");
}

$test = $T->SpawnWaitKill (15);

if ($test != 0)
{
    print STDERR "ERROR: Bug_2137_Regression test returned $test\n";
    exit 1;
}
else
{
    print STDERR "SUCCESS: Bug_2137_Regression test returned $test\n";
}

exit 0;
