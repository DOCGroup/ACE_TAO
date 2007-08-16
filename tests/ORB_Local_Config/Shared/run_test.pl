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
    my $t1 = new PerlACE::Process ($executable, ($arguments ? $arguments : ""));
    my $status = $t1->SpawnWaitKill (10);
    if ($status != 0) {
        print STDERR "ERROR: test failed, status=$status\n";
    }
    return $status;
} 

my $status = 0;
$status |= test("Test");

if ($status == 0) {
    print STDERR "SUCCESS: All tests passed\n";
}
else {
    print STDERR "ERROR: Some test failed, status=$status\n";
}
exit $status;

