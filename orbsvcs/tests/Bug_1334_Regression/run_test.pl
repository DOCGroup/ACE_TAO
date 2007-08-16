
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The NameService IOR file
#
$nsiorfile = PerlACE::LocalFile("ns.ior");
unlink $nsiorfile;

# The client process
#
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("client"));

# Fire up the Name Service
#
$NS   = new PerlACE::Process ("../../Naming_Service/Naming_Service", "-o $nsiorfile");
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsiorfile, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find Name Service IOR file <$nsiorfile>\n";
   $NS->Kill (); $NS->TimedWait (1);
   exit 1;
}


# Try the client with the name service ior in an environment variable of
# the form <service name>IOR.
# Expect it to succeed.
#
$ENV {'NameServiceIOR'} = "file://$nsiorfile";
$CLIENT->Arguments("");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: Name Service not resolved by environment variable\n";
   $server = $NS->TerminateWaitKill (5);
   if ($server != 0)
   {
       print STDERR "ERROR: Closing Name Service returned $server\n";
       $status = 1;
   }
   unlink $nsiorfile;
   exit 1;
}

# Now try it with a duff ior in the environment variable.
# Expect it to fail.
#
$ENV {'NameServiceIOR'} = "Banana";
$CLIENT->Arguments("");
if ($CLIENT->SpawnWaitKill (30) != 1)
{
   print STDERR "ERROR: Name Service resolved with duff environment\n";
   $server = $NS->TerminateWaitKill (5);
   if ($server != 0)
   {
       print STDERR "ERROR: Closing Name Service returned $server\n";
       $status = 1;
   }
   unlink $nsiorfile;
   exit 1;
}

# Now try with a duff ior in the environment variable but overridden by
# a command line parameter.
# Expect it to succeed.
#
$ENV {'NameServiceIOR'} = "Custard";
$CLIENT->Arguments("-ORBInitRef NameService=file://$nsiorfile");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: Name Service not resolved by command line override\n";
   $server = $NS->TerminateWaitKill (5);
   if ($server != 0)
   {
       print STDERR "ERROR: Closing Name Service returned $server\n";
       $status = 1;
   }
   unlink $nsiorfile;
   exit 1;
}

# Clean up and return
#
$server = $NS->TerminateWaitKill (5);
if ($server != 0)
{
    print STDERR "ERROR: Closing Name Service returned $server\n";
    $status = 1;
}
unlink $nsiorfile;

exit 0;
