
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The server IOR file
$server_ior_file = PerlACE::LocalFile ("server.ior");

# The client and server processes
$SERVER     = new PerlACE::Process(PerlACE::LocalFile("server"));
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("client"));

# We want the server to run on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

# We would normally have to specify the server port / host via -ORBEndpoint
# but we are testing the ability to define it as an environment property
$ENV {'TAO_ORBENDPOINT'} = "iiop://localhost:$port";

# Fire up the server
$SERVER->Spawn();

# We don't need the IOR file but we can wait on the file
if (PerlACE::waitforfile_timed ($server_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find $server_ior_file\n";
   $SERVER->Kill();
   exit 1;
}

# Try the corbaloc URL for the server. It's almost impossible that this will work
# if the endpoint hasn't been read from the environment so we call success a pass.
$CLIENT->Arguments("-k corbaloc:iiop:localhost:$port/Name-with-hyphens");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: Bug rt596 Regression failed. Endpoint was not read from environment\n";
   $SERVER->Kill();
   exit 1;
}

# Clean up and return
$SERVER->TerminateWaitKill (5);
unlink $server_ior_file;
exit 0;
