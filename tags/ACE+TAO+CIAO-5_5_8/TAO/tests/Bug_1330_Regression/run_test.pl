
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The server IOR file
$iorbase = "server.ior";

# The client and server processes
if (PerlACE::is_vxworks_test()) {
  $SERVER     = new PerlACE::ProcessVX("server");
  $server_ior_file = $iorbase;
  $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
}
else {
  $SERVER     = new PerlACE::Process("server");
  $server_ior_file = PerlACE::LocalFile ($iorbase);
  $TARGETHOSTNAME = "localhost";
}
unlink $server_ior_file;

$CLIENT     = new PerlACE::Process("client");

# We want the server to run on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0
$SERVER->Arguments("-ORBEndpoint iiop://:$port");

# Fire up the server
$SERVER->Spawn();

# We don't need the IOR file but we can wait on the file
if (PerlACE::waitforfile_timed ($server_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find $server_ior_file\n";
   $SERVER->Kill();
   exit 1;
}

# Try the corbaloc URL with incorrect '\' escaping of hex characters
# We expect this to 'fail'
$CLIENT->Arguments("-k \"corbaloc:iiop:$TARGETHOSTNAME:$port/Name\\2dwith\\2dhyphens\"");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: Bug 1330 Regression failed. Correct escape characters rejected\n";
   $SERVER->Kill();
   exit 1;
}

# Try the corbaloc URL with the correct '%' escaping of hex characters
# We expect success
$CLIENT->Arguments("-k corbaloc:iiop:$TARGETHOSTNAME:$port/Name%2dwith%2dhyphens");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: Bug 1330 Regression failed. Correct escape characters rejected\n";
   $SERVER->Kill();
   exit 1;
}

# Clean up and return
$SERVER->TerminateWaitKill (5);
unlink $server_ior_file;
exit 0;
