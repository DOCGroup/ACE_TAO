eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

sub add_path {
  my($name)  = shift;
  my($value) = shift;
  if (defined $ENV{$name}) {
    $ENV{$name} .= ':' . $value
  }
  else {
    $ENV{$name} = $value;
  }
}

# Set the library path for the client to be able to load
# the Time_Date library.
add_path('LD_LIBRARY_PATH', '../lib');
add_path('LIBPATH', '../lib');
add_path('SHLIB_PATH', '../lib');

sub test($)
{
    (my $executable, my $arguments) = @_;
    chdir ($executable);
    my $t1 = new PerlACE::Process ("Test", ($arguments ? $arguments : ""));
    print STDERR "\nTest $executable is running ...\n";
    my $status = $t1->SpawnWaitKill (10);
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

