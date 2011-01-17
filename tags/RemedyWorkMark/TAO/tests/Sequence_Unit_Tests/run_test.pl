eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $final_result = 0;

my @testsToRun = qw(unbounded_value_sequence_ut
               bounded_value_sequence_ut
               string_sequence_element_ut
               unbounded_string_sequence_ut
               bounded_string_sequence_ut
               testing_allocation_traits_ut
               unbounded_octet_sequence_ut
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

if ($^O ne "MSWin32") {
    push @testsToRun, qw(unbounded_octet_sequence_nocopy_ut);
}

foreach my $process (@testsToRun) {

    my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
    my $P = $server->CreateProcess ($process);
    print "Running $process ...\n";
    my $result = $P->Spawn;
    if ($result != 0) {
        print "FAILED $process\n";
        $final_result = 1;
        next;
    }
    $result = $P->WaitKill($server->ProcessStartWaitInterval());
    if ($result != 0) {
        print "FAILED $process\n";
        $final_result = 1;
        next;
    }
    print "SUCCESS\n";
}

exit $final_result;
