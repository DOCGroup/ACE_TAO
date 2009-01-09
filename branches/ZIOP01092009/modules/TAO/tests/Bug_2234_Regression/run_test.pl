eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The server IOR file
$server_ior_file= PerlACE::LocalFile( "server.ior" );
unlink $server_ior_file;

# The client and server processes
my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
$SERVER = new $class ("server");

$CLIENT= new PerlACE::Process( PerlACE::LocalFile( "client" ) );

# Fire up the server
$sv = $SERVER->Spawn();

if ($sv != 0) {
   print STDERR "ERROR: server returned $sv\n";
   exit 1;
}
# We can wait on the IOR file
if (PerlACE::waitforfile_timed( $server_ior_file, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find $server_ior_file\n";
   $SERVER->Kill();
   exit 1;
}

if ($CLIENT->SpawnWaitKill( 30 ) != 0)
{
   unlink $server_ior_file;
   print STDERR "ERROR: Bug #2234 Regression test failed: cannot run client.\n";
   $SERVER->Kill();
   exit 1;
}
unlink $server_ior_file;

if ($SERVER->WaitKill( 15 ) != 0)
{
   print STDERR "ERROR: Bug #2234 Regression test failed: server not exiting.\n";
   exit 1;
}

print STDOUT "Bug #2234 Regression test succeeded.\n";
exit 0;
