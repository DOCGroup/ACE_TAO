eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The location of the implementation repository binaries
$ifr_bin_path = "../../IFR_Service";

# The location of the tao_ifr IFR utility
$tao_ifr_bin_path = "$ENV{ACE_ROOT}/bin";

# IOR file names
$ifr_ior_file = PerlACE::LocalFile("ifr.ior");

# IDL File
$idl_file = PerlACE::LocalFile("test.idl");

#Log file
$result_file = PerlACE::LocalFile("test_result.log");

$IFRSERVICE = new PerlACE::Process("$ifr_bin_path/IFR_Service");
$TAO_IFR    = new PerlACE::Process("$tao_ifr_bin_path/tao_ifr");

sub test_body
{
   unlink $ifr_ior_file;

   # Start the IFR Service to generate an IOR file for the tao_ifr to use...
   $IFRSERVICE->Arguments("-o $ifr_ior_file ");
   $IFRSERVICE->Spawn ();

   if (PerlACE::waitforfile_timed ($ifr_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
   {
      print STDERR "ERROR: cannot find $ifr_ior_file\n";
      $IFRSERVICE->Kill ();
      return 1;
   }

   # Redirect STDERR to a log file so that
   # we can make sure that we got a warning
   open(SAVEERR, ">&STDERR");
   open(STDERR, ">$result_file");

   $TAO_IFR->Arguments("-ORBInitRef InterfaceRepository=file://$ifr_ior_file -Cw $idl_file");
   $TAO_IFR->SpawnWaitKill (30);

   # Close the log file and restore STDERR
   close(STDERR);
   open(STDERR, ">&SAVEERR");

   if (! -r $result_file) {
      print STDERR "ERROR: cannot find $result_file\n";
      $IFRSERVICE->Kill ();
      $TAO_IFR->Kill ();
      return 1;
   }

   $match = 0;
   open (FILE, $result_file) or return -1;
   while (<FILE>) {
       $match = /Warning - identifier spellings differ only in case:/;
       last if $match;
      }
   close FILE;
   # Tidy up
   $IFRSERVICE->TerminateWaitKill (5);
   return $match ? 0 : -1;
}

# Run regression for bug #1436
$test_result = test_body();

if ($test_result != 0)
{
   print STDERR "ERROR: Regression test for Bug #1436 failed\n";
}
else
{
   print "Regression test for Bug #1436 passed.\n";
}

unlink $ifr_ior_file;
unlink $result_file;
exit $test_result;
