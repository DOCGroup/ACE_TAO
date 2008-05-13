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

# Set the library path for the client to be able to load the library
PerlACE::add_lib_path ('../lib');

sub test($)
{
    (my $executable, my $arguments) = @_;
    my $t1;
    if (PerlACE::is_vxworks_test()) {
      $t1 = new PerlACE::ProcessVX ($executable, ($arguments ? $arguments : ""));
    }
    else {
      $t1 = new PerlACE::Process ($executable, ($arguments ? $arguments : ""));
    }
    my $status = $t1->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
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

