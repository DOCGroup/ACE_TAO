eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

sub get_test_modes
{
  my $testid = shift;

  my $client_mode = "LOCAL_AND_REMOTE";
  my $server_mode = "LOCAL_AND_REMOTE";

  # There are Nine Permutations.
  if ($testid == 1)
  {
    $client_mode = "LOCAL_AND_REMOTE";
    $server_mode = "LOCAL_AND_REMOTE";
  }
  elsif ($testid == 2)
  {
    $client_mode = "LOCAL_AND_REMOTE";
    $server_mode = "LOCAL_ONLY";
  }
  elsif ($testid == 3)
  {
    $client_mode = "LOCAL_AND_REMOTE";
    $server_mode = "REMOTE_ONLY";
  }
  elsif ($testid == 4)
  {
    $client_mode = "LOCAL_ONLY";
    $server_mode = "LOCAL_AND_REMOTE";
  }
  elsif ($testid == 5)
  {
    $client_mode = "LOCAL_ONLY";
    $server_mode = "LOCAL_ONLY";
  }
  elsif ($testid == 6)
  {
    $client_mode = "LOCAL_ONLY";
    $server_mode = "REMOTE_ONLY";
  }
  elsif ($testid == 7)
  {
    $client_mode = "REMOTE_ONLY";
    $server_mode = "LOCAL_AND_REMOTE";
  }
  elsif ($testid == 8)
  {
    $client_mode = "REMOTE_ONLY";
    $server_mode = "LOCAL_ONLY";
  }
  elsif ($testid == 9)
  {
    $client_mode = "REMOTE_ONLY";
    $server_mode = "REMOTE_ONLY";
  }
  else
  {
    print STDERR "ERROR: invalid testid: $testid\n";
    exit 1;
  }

  return ($client_mode, $server_mode);
}

my $status = 0;
my $file = PerlACE::LocalFile ("test.ior");

my $testid;

for ($testid = 1; $testid <= 9; ++$testid)
{
  unlink $file;

  my $client_mode;
  my $server_mode;

  ($client_mode, $server_mode) = get_test_modes($testid);

  my $SV;
  if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("PI_ProcMode_Collocated_Test",
                                  "-c $client_mode " .
                                  "-s $server_mode " .
                                  "-ORBobjrefstyle url");
  }
  else {
    $SV = new PerlACE::Process ("PI_ProcMode_Collocated_Test",
                                "-c $client_mode " .
                                "-s $server_mode " .
                                "-ORBobjrefstyle url");
  }

  print STDERR "\n\n==== Starting test variant #$testid\n\n";

  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find file <$file>\n";
      $SV->Kill ();
      exit 1;
  }

  my $collocated = $SV->WaitKill (5);

  if ($collocated != 0) {
      print STDERR "ERROR: PI_ProcMode_Collocated_Test returned $collocated\n";
      print STDERR "ERROR: For client-side mode [$client_mode],\n";
      print STDERR "ERROR: and server-side mode [$server_mode].\n";
      $status = 1;
      print STDERR "\nTest variant #$testid of 9 failed!\n\n";
  }
  else {
      print STDERR "\nTest variant #$testid of 9 passed!\n\n";
  }
}

unlink $file;

if ($status == 0) {
    print STDERR "\n==== All 9 test variants were successful!\n";
}
else {
    print STDERR "\n==== One or more of the 9 test variants failed!\n";
}

exit $status;
