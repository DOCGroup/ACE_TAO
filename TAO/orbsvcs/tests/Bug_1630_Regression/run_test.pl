
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# The IFRService IOR file
#
$ifriorfile = PerlACE::LocalFile("ifr.ior");
unlink $ifriorfile;

# The client process
#
$CLIENT     = new PerlACE::Process(PerlACE::LocalFile("testclient"));

# Fire up the IFR Service
#
$IFR   = new PerlACE::Process ("../../IFR_Service/IFR_Service", "-o $ifriorfile");
$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($ifriorfile, $PerlACE::wait_interval_for_process_creation) == -1)
{
   print STDERR "ERROR: cannot find IFR Service IOR file <$ifriorfile>\n";
   $IFR->Kill (); $IFR->TimedWait (1);
   exit 1;
}


# Run the client, it should return 0 for success.
$CLIENT->Arguments("-ORBInitRef InterfaceRepository=file://$ifriorfile");
if ($CLIENT->SpawnWaitKill (30) != 0)
{
   print STDERR "ERROR: test failed\n";
   $server = $IFR->TerminateWaitKill (5);
   if ($server != 0)
   {
       print STDERR "ERROR: Closing IFR Service returned $server\n";
       $status = 1;
   }
   unlink $ifriorfile;
   exit 1;
}

# Clean up and return
#
$server = $IFR->TerminateWaitKill (5);
if ($server != 0)
{
    print STDERR "ERROR: Closing IFR Service returned $server\n";
    $status = 1;
}
unlink $ifriorfile;

exit 0;
