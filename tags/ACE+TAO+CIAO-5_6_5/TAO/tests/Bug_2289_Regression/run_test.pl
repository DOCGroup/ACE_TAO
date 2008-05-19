eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The server IOR file
$server_ior_file = PerlACE::LocalFile ("server.ior");
$client_ior_file = PerlACE::LocalFile ("client.ior");
unlink $server_ior_file;
unlink $client_ior_file;

# The client and server processes
if (PerlACE::is_vxworks_test()) {
  $SERVER     = new PerlACE::ProcessVX("server");
  $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
}
else {
  $SERVER     = new PerlACE::Process("server");
  $TARGETHOSTNAME = "127.0.0.1";
}
$CLIENT     = new PerlACE::Process("client");

# We want the server to run on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

$SERVER->Arguments("-ORBEndpoint iiop://:$port -ORBDottedDecimalAddresses 1");

# Fire up the server
$sv = $SERVER->Spawn();

if ($sv != 0) {
   print STDERR "ERROR: server returned $sv\n";
   exit 1;
}

# We can wait on the IOR file
if (PerlACE::waitforfile_timed ($server_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find $server_ior_file\n";
   $SERVER->Kill();
   exit 1;
}

$CLIENT->Arguments("-k corbaloc::$TARGETHOSTNAME:$port/collocated_ior_bound_in_remote_iortable -ORBDottedDecimalAddresses 1 -ORBCollocationStrategy thru_poa");
if ($CLIENT->SpawnWaitKill (60) != 0)
{
   print STDERR "ERROR: Bug 2289 Regression failed. Non zero result from client.\n";
   $SERVER->Kill();
   exit 1;
}

# Clean up and return
$SERVER->TerminateWaitKill (15);
unlink $server_ior_file;
unlink $client_ior_file;
exit 0;
