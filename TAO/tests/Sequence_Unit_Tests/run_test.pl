eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use strict;

my $final_result = 0;

foreach my $process (qw(unbounded_value_sequence_ut
                        bounded_value_sequence_ut
                        Unbounded_Simple_Types
                        Bounded_Simple_Types)) {

  # Not all the binaries are generated in all configurations.
  next unless -x $process;

  print "Running $process ...";
  my $P = new PerlACE::Process ($process,
                                '--log_level=nothing '
                                .'--report_level=no');
  my $result = $P->SpawnWaitKill(30);
  if ($result != -1) {
    print "SUCCESS\n";
  } else {
    print "FAILED\n";
    $final_result = 1;
  }
}

exit $final_result;
