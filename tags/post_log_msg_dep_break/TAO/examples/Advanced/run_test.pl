eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
require ACEutils;
require Process;

$status = 0;


sub run_test_from_current_directory{
  announce_test();
  $test = Process::Create($EXEPREFIX."run_test.pl","");
  wait_for_test_to_finish();
}

sub announce_test{
  use Cwd;
  $dir = cwd();
  print STDOUT "__________________________________________________________ \n";
  print STDOUT "**running test in $dir : \n";
}

sub wait_for_test_to_finish {
  if ($test->TimedWait (60) == -1) {
    print STDERR "ERROR: run_test.pl timedout\n";
    $status = 1;
    $test->Kill (); $test->TimedWait (1);
  }
}




# change to each example's directory, and run their test
chdir "ch_3" or die "can't cd to ch_3 $!\n";
run_test_from_current_directory();

chdir "../ch_8_and_10" or die "can't cd to ch_8_and_10 $!\n";
run_test_from_current_directory();

chdir "../ch_12" or die "can't cd to ch_12 $!\n";
run_test_from_current_directory();

chdir "../ch_18" or die "can't cd to ch_18 $!\n";
run_test_from_current_directory();

#chdir "../ch_19" or die "can't cd to ch_19 $!\n";
#run_test_from_current_directory();

#chdir "../ch_21" or die "can't cd to ch_21 $!\n";
#run_test_from_current_directory();

exit $status;




