eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# Set the library path for the client to be able to load
# the Time_Date library.
PerlACE::add_lib_path ('../lib');

sub test($)
{
    (my $executable, my $arguments) = @_;
    chdir ($executable);
    my $t1;
    if (PerlACE::is_vxworks_test()) {
      $t1 = new PerlACE::ProcessVX ("Test", ($arguments ? $arguments : ""));
    }
    else {
      $t1 = new PerlACE::Process ("Test", ($arguments ? $arguments : ""));
    }
    print STDERR "\nTest $executable is running ...\n";
    my $status = $t1->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
    chdir ("..");
    if ($status != 0) {
        print STDERR "\nERROR: Test $executable failed, status=$status\n";
        return -1;
    }

    print STDERR "Test $executable reported success.\n";
    return 0;
} 

my $status = 0;
$status += test("Bug_1459");
$status += test("Bug_2612");
$status += test("Bunch");
$status += test("Separation");
$status += test("Service_Dependency");
$status += test("Shared");
$status += test("Simple");
$status += test("Two_DLL_ORB");

if ($status == 0) {
    print STDERR "SUCCESS: All tests passed\n";
}
else {
    print STDERR "ERROR: Some test failed, status=$status\n";
}
exit $status;

