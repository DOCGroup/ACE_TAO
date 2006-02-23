eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use strict;

my $final_result = 0;

my @tests = qw(unbounded_value_sequence_ut
               unbounded_array_sequence_ut
               bounded_value_sequence_ut
               string_sequence_element_ut
               unbounded_string_sequence_ut
               bounded_string_sequence_ut
               testing_allocation_traits_ut
               unbounded_octet_sequence_ut
               unbounded_octet_sequence_no_copy_ut
               object_reference_sequence_element_ut
               unbounded_object_reference_sequence_ut
               unbounded_fwd_object_reference_sequence_ut
               bounded_object_reference_sequence_ut
               bounded_sequence_cdr_ut
               unbounded_sequence_cdr_ut
               Unbounded_Octet
               Unbounded_Simple_Types
               Bounded_Simple_Types
               Unbounded_String
               Bounded_String
              );

foreach my $process (@tests) {

    
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
  $result = $P->WaitKill(10);
  if ($result != 0) {
    print "FAILED\n";
    $final_result = 1;
    next;
  }
  print "SUCCESS\n";
}

exit $final_result;
