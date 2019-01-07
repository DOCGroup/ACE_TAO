eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use strict;
use Env qw(ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

my $annotest_idl = new PerlACE::Process("annotest_idl", "");
print $annotest_idl->CommandLine ();
$annotest_idl->Spawn ();
my $annotest_idl_result = $annotest_idl->WaitKill (10);
if ($annotest_idl_result > 0) {
  print STDERR "ERROR: annotest_idl returned $annotest_idl_result\n";
}

my $status = 1 if $annotest_idl_result;

if ($status) {
  print STDERR "test FAILED";
}
else {
  print "test PASSED";
}

exit $status
