eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib '../../../../bin';
use PerlACE::Run_Test;

# The location of the implementation repository binaries
$imr_bin_path  = "../../ImplRepo_Service";

# The location of the tao_imr IMR utility 
if ($^O eq "MSWin32") 
{
   $tao_imr_bin_path = "../../../../bin";
}
else 
{
   $tao_imr_bin_path = $imr_bin_path;
}

# IOR file names
$imr_ior_file        = PerlACE::LocalFile("impl.ior");
$activator_ior_file  = PerlACE::LocalFile("activator.ior");
$server_ior_file     = PerlACE::LocalFile("server.ior");

# The players in our little drama.
$LOCATOR    = new PerlACE::Process("$imr_bin_path/ImplRepo_Service");
$ACTIVATOR  = new PerlACE::Process("$imr_bin_path/ImR_Activator");
$SERVER     = new PerlACE::Process(PerlACE::LocalFile("server"));
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("client"));
$TAO_IMR    = new PerlACE::Process("$tao_imr_bin_path/tao_imr");

# Run the IMR locator on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

sub test_body
{
   unlink $imr_ior_file;
   unlink $activator_ior_file;
   unlink $server_ior_file;
   
   # Start the IMR locator to generate an IOR file for the server to use...
   $LOCATOR->Arguments("-o $imr_ior_file -ORBEndpoint iiop://:$port");
   $LOCATOR->Spawn ();
   
   if (PerlACE::waitforfile_timed ($imr_ior_file, 10) == -1) 
   {
      print STDERR "ERROR: cannot find $imr_ior_file\n";
      $LOCATOR->Kill ();
      return 1;
   }
   
   # ...then shut it down so that the server will not be able to contact it 
   $LOCATOR->TerminateWaitKill (5);
   
   if ($imr_locator != 0) 
   {
      print STDERR "ERROR: IMR returned $imr_locator\n";
      $status = 1;
   }
   
   # Start our server
   $SERVER->Arguments("-o $server_ior_file -ORBInitRef ImplRepoService=file://$imr_ior_file -ORBUseIMR 1");
   $SERVER->Spawn ();
   
   if (PerlACE::waitforfile_timed ($server_ior_file, 10) == -1) 
   {
      print STDERR "ERROR: cannot find $server_ior_file\n";
      $ACTIVATOR->Kill ();
      $LOCATOR->Kill ();
      $SERVER->Kill ();
      return 1;
   }
   
   # Use the client to ask the server to try and create a persitent POA
   # We expect this to 'fail' as the IMR is dead
   $CLIENT->Arguments("-k file://$server_ior_file");
   $result = $CLIENT->SpawnWaitKill (30);
   
   if ($result == 0) 
   {
      print STDERR "ERROR: First create POA attempt succeeded when it shouldn't have done\n";
      $SERVER->Kill ();
      return 1;
   }
   
   # Now we restart the IMR locator
   unlink $imr_ior_file;
   $LOCATOR->Arguments("-o $imr_ior_file -ORBEndpoint iiop://:$port");
   $LOCATOR->Spawn ();
   
   if (PerlACE::waitforfile_timed ($imr_ior_file, 10) == -1) 
   {
      print STDERR "ERROR: cannot find $imr_ior_file\n";
      $LOCATOR->Kill ();
      return 1;
   }
   
   # Work out the IMR activator command line.
   # Use the '-a' switch if this is a regression for bug #1394, else not
   if ($use_tao_imr_util != 0)
   {
      $activator_arguments = "-o $activator_ior_file -ORBInitRef ImplRepoService=file://$imr_ior_file";
   }
   else
   {  
      $activator_arguments = "-o $activator_ior_file -a -ORBInitRef ImplRepoService=file://$imr_ior_file";
   }
   
   # Start up the activator
   $ACTIVATOR->Arguments ($activator_arguments);
   $ACTIVATOR->Spawn ();
   
   if (PerlACE::waitforfile_timed ($activator_ior_file, 10) == -1) 
   {
      print STDERR "ERROR: cannot find $activator_ior_file\n";
      $ACTIVATOR->Kill ();
      $LOCATOR->Kill ();
      return 1;
   }
   
   # If this is just a regression for bug #1395 we need to register the POA
   # If it is a regression for enhancement bug #1394, we don't need to.
   if ($use_tao_imr_util != 0)
   {
      # Add the persistent POA name to the IMR
      $TAO_IMR->Arguments("add MyPoa -ORBInitRef ImplRepoService=file://$imr_ior_file");
      $result = $TAO_IMR->SpawnWaitKill (30);
      
      if ($result != 0) 
      {
         print STDERR "ERROR: tao_imr returned $result\n";
         $ACTIVATOR->Kill ();
         $LOCATOR->Kill ();
         return 1;
      }
   }
   
   # Use the client to tell the server to attempt to create the POA again
   $CLIENT->Arguments("-k file://$server_ior_file");
   $result = $CLIENT->SpawnWaitKill (30);
   
   if ($result != 0) 
   {
      print STDERR "ERROR: Second create POA attempt failed when it should have succeeded\n";
      $SERVER->Kill ();
      $ACTIVATOR->Kill ();
      $LOCATOR->Kill ();
      return 1;
   }
   
   # Tidy up
   $SERVER->TerminateWaitKill (5);
   $ACTIVATOR->TerminateWaitKill (5);
   $LOCATOR->TerminateWaitKill (5);
   return 0;
}   

# Run regression for bug #1395
$use_tao_imr_util = 1;
$test_result = test_body(); 
if ($test_result != 0)
{
   print STDERR "ERROR: Regression test for Bug #1395 failed\n";
   unlink $imr_ior_file;
   unlink $activator_ior_file;
   unlink $server_ior_file;
   exit $test_result;
}

# Bug 1394 is an enhancement so will not be submitted until after TAO1.3.1
# Uncomment the following to activate regression after submission and ..
# Run regression for bug #1394
#$use_tao_imr_util = 0;
#$test_result = test_body(); 
#if ($test_result != 0)
#{
#   print STDERR "ERROR: Regression test for Bug #1394 failed\n";  
#}

unlink $imr_ior_file;
unlink $activator_ior_file;
unlink $server_ior_file;   
exit $test_result;
