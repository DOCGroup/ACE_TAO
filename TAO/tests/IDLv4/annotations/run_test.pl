eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use strict;
use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test

my $status = 0;

my $annotest_idl = PerlDDS::create_process ("annotest_idl", "");
print $annotest_idl->CommandLine ();
$annotest_idl->Spawn ();
my $annotest_idl_result = $annotest_idl->WaitKill (10);
if ($annotest_idl_result > 0)
  {
    print STDERR "ERROR: annotest_idl returned $annotest_idl_result\n";
  }
$status = 1 if $annotest_idl_result;

if ($status)
  {
    print STDERR "test FAILED";
  }
else
  {
    print "test PASSED";
  }

exit $status
