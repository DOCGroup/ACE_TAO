eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $process = 'AST_Unit_Tests';
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $P = $server->CreateProcess ($process);

print "Running $process ...\n";
my $result = $P->Spawn;
if ($result != 0) {
  print "FAILED $process\n";
  exit 1;
}

$result = $P->WaitKill ($server->ProcessStartWaitInterval ());
if ($result != 0) {
  print "FAILED $process\n";
  exit 1;
}

print "SUCCESS\n";
exit 0;
