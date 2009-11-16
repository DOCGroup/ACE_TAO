eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The location of the implementation repository binaries
$imr_bin_path  = "../../ImplRepo_Service";

# The location of the tao_imr IMR utility
$tao_imr_bin_path = "$ENV{ACE_ROOT}/bin";

# IOR file names
$imr_ior_file        = PerlACE::LocalFile("impl.ior");
$activator_ior_file  = PerlACE::LocalFile("activator.ior");

# Log file
$log_file  = PerlACE::LocalFile("test_result.log");

# The players in our little drama.
$LOCATOR    = new PerlACE::Process("$imr_bin_path/ImplRepo_Service");
$ACTIVATOR  = new PerlACE::Process("$imr_bin_path/ImR_Activator");
$TAO_IMR    = new PerlACE::Process("$tao_imr_bin_path/tao_imr");

# Run the IMR locator on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

sub test_body
{
   unlink $imr_ior_file;
   unlink $activator_ior_file;
   unlink $log_file;

   # Start the IMR locator to generate an IOR file for the server to use...
   $LOCATOR->Arguments("-o $imr_ior_file -ORBEndpoint iiop://:$port");
   $LOCATOR->Spawn ();

   if (PerlACE::waitforfile_timed ($imr_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
   {
      print STDERR "ERROR: cannot find $imr_ior_file\n";
      $LOCATOR->Kill ();
      return 1;
   }

   # Set the activator arguments
   $activator_arguments = "-o $activator_ior_file -ORBInitRef ImplRepoService=file://$imr_ior_file";

   # Start up the activator
   $ACTIVATOR->Arguments ($activator_arguments);
   $ACTIVATOR->Spawn ();

   if (PerlACE::waitforfile_timed ($activator_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
   {
      print STDERR "ERROR: cannot find $activator_ior_file\n";
      $ACTIVATOR->Kill ();
      $LOCATOR->Kill ();
      return 1;
   }

   # Redirect STDERR to a log file so that the ERROR
   # message does not get printed to the terminal
   open(SAVEERR, ">&STDERR");
   open(STDERR, ">$log_file");

   # Add the illegal persistent POA name to the IMR
   $TAO_IMR->Arguments("-ORBInitRef ImplRepoService=file://$imr_ior_file add \"\"");
   $result = $TAO_IMR->SpawnWaitKill (30);

   # Close the log file and restore STDERR
   close(STDERR);
   open(STDERR, ">&SAVEERR");

   # If the add of an empty string failed, then
   # the test of the tao_imr succeeded.
   if ($result != 0) {
     $result = 0;
   }
   else {
      $result = -1;
   }

   # Tidy up
   $ACTIVATOR->TerminateWaitKill (5);
   $LOCATOR->TerminateWaitKill (5);
   return $result;
}

# Run regression for bug #1437
$test_result = test_body();

if ($test_result != 0)
{
   print STDERR "ERROR: Regression test for Bug #1437 failed\n";
}
else
{
   print "Regression test for Bug #1437 passed.\n";
}
unlink $log_file;
unlink $imr_ior_file;
unlink $activator_ior_file;

exit $test_result;
