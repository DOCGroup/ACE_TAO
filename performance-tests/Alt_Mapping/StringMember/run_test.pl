eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $test = PerlACE::TestTarget::create_target(1) || die "Create target failed\n";

$num = 10000;
$len = 40;
$copy = 0;
$status = 0;

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
      print "Run_Test Perl script for TAO StringMember Test\n\n";
      print "run_test [-n num] [-l length]\n";
      print "\n";
      print "-n num              -- num iterations for stats\n";
      print "-l length           -- string member length\n";
      print "-c                  -- toggle copying string assignment\n";
      exit 0;
    }
    elsif ($ARGV[$i] eq "-n") {
      $num = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-l") {
      $len = $ARGV[$i + 1];
      $i++;
    }
    elsif ($ARGV[$i] eq "-c") {
      $copy = 1;
    }
}

$TEST = $test->CreateProcess ("test");

if ($copy == 1) {
  $TEST->Arguments ("-n $num -l $len -c");
}
else {
  $TEST->Arguments ("-n $num -l $len");
}

$test_status = $TEST->SpawnWaitKill (10);

if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    $status = 1;
}

exit $status;
