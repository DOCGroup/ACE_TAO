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

my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
$SV = new $class ("bug2936");

print STDERR "\n\n==== Running bug 2936 regression test\n";

$SV->Spawn ();

$collocated = $SV->WaitKill (30);

if ($collocated != 0) {
    print STDERR "ERROR: Bug_2936_Regression returned $collocated\n";
    $status = 1;
}

unlink $file;

exit $status;
