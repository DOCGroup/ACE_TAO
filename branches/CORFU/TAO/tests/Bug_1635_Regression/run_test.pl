
# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The file we want TAO to write debug messages to
$debug_file = PerlACE::LocalFile ("debug.log");
unlink $debug_file;

# The client process - simply initialises an ORB to create some minimal debug
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("client"));
$CLIENT->Arguments("-ORBDebugLevel 10 -ORBLogFile $debug_file");

sub run_client
{
   # Run the client
   if ($CLIENT->SpawnWaitKill (30) != 0)
   {
      print STDERR "ERROR: Bug 1635 Regression failed. Client failed to run.\n";
      $ENV {'TAO_LOG_TIMESTAMP'} = "";
      exit 1;
   }

   # Open the log file, read a line, close, and delete it.
   open (HANDLE, "$debug_file");
   $_ = <HANDLE>;
   close HANDLE;
   unlink $debug_file;
}

# Run the client with no value for the environment variable
$ENV {'ACE_LOG_TIMESTAMP'} = "";
run_client;

# Check result is as expected, i.e. starts like:
# TAO (12...etc.
if (!/^TAO \(/)
{
   print STDERR "ERROR: Bug 1635 Regression failed. Unexpected normal log format.\n";
   $ENV {'ACE_LOG_TIMESTAMP'} = "";
   exit 1;
}

# Try again with the 'TIME' logging property set
$ENV {'ACE_LOG_TIMESTAMP'} = "TIME";
run_client;

# Check result is as expected, i.e. starts like:
# 14:36:38.222000|TAO (12...etc.
if (!/^\d{2}:\d{2}:\d{2}.\d{6}\|TAO \(/)
{
   print STDERR "ERROR: Bug 1635 Regression failed. Unexpected 'TIME' log format.\n";
   $ENV {'ACE_LOG_TIMESTAMP'} = "";
   exit 1;
}

# Try again with the 'DATE' logging property set
$ENV {'ACE_LOG_TIMESTAMP'} = "DATE";
run_client;

# Check result is as expected, i.e. starts like:
# Wed Feb 12 2003 14:36:38.222000|TAO (12...etc.
if (!/^[A-Z][a-z]{2} [A-Z][a-z]{2} \d{2} \d{4} \d{2}:\d{2}:\d{2}.\d{6}\|TAO \(/)
{
  print STDERR "ERROR: Bug 1635 Regression failed. Unexpected 'DATE' log format.\n";
  $ENV {'ACE_LOG_TIMESTAMP'} = "";
  exit 1;
}

$ENV {'ACE_LOG_TIMESTAMP'} = "";
exit 0;

