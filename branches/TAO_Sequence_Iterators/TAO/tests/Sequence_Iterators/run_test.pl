eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

if ($ARGV[0] =~ /^-h/i || $ARGV[0] =~ /^-\?/i) {
  print "Usage: run_test.pl [-boost|-noboost]\n".
      "\tDefault is to run all tests, specifying -boost or -noboost will\n".
      "\teither run the tests that require the boost unit test library or\n".
      "\tthe other tests, respectively.\n";
  exit 0;
}

my $final_result = 0;

my @testsToRun = qw(
                 StringSeq
                 Bounded_String
                 Unbounded_Value
                 Unbounded_Objectref
              );

foreach my $process (@testsToRun) {

  my $P = 0;
  if (PerlACE::is_vxworks_test()) {
      $P = new PerlACE::ProcessVX ($process,
                                  '--log_level=nothing '
                                  .'--report_level=no');
  }
  else {
      $P = new PerlACE::Process ($process,
                                '--log_level=nothing '
                                .'--report_level=no');
  }
  my $executable = $P->Executable;

  # Not all the binaries are generated in all configurations.
  if (PerlACE::is_vxworks_test()) {
    next unless -e $executable;
  }
  else {
    next unless -x $executable;
  }

  print "Running $process ...";
  my $result = $P->Spawn;
  if ($result != 0) {
    print "FAILED\n";
    $final_result = 1;
    next;
  }
  $result = $P->WaitKill($PerlACE::wait_interval_for_process_creation);
  if ($result != 0) {
    print "FAILED\n";
    $final_result = 1;
    next;
  }
  print "SUCCESS\n";
}

exit $final_result;
