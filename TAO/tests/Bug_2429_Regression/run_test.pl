eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

#$Id$

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The server IOR file
$server_ior_file = PerlACE::LocalFile ("server.ior");
unlink $server_ior_file;

# The client and server processes
$SERVER     = new PerlACE::Process(PerlACE::LocalFile("server"));
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("client"));

$SERVER->Arguments("-o $server_ior_file");

# Fire up the server
$SERVER->Spawn();

# We can wait on the IOR file
if (PerlACE::waitforfile_timed ($server_ior_file, 10) == -1)
{
   print STDERR "ERROR: cannot find $server_ior_file\n";
   $SERVER->Kill();
   exit 1;
}

$CLIENT->Arguments("-f file://$server_ior_file");
if ($CLIENT->SpawnWaitKill (10) != 0)
{
   print STDERR "ERROR: Regression - Non zero result from client or client timed out.\n";
   $SERVER->Kill();
   exit 1;
}

# Clean up and return
$SERVER->TerminateWaitKill (5);
unlink $server_ior_file;
exit 0;
