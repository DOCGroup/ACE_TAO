eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("Worker.ior");
$nsior = PerlACE::LocalFile ("ns.ior");
$rmior = PerlACE::LocalFile ("rm.ior");
$hmior = PerlACE::LocalFile ("hm.ior");
$hmutil = PerlACE::LocalFile ("hm-util.dat");

unlink $iorfile;
unlink $nsior;
unlink $rmior;
unlink $hmior;
unlink $hmutil;

$status = 0;

$ns_endpoint = "iiop://localhost:45452";
$ns_initref = "-ORBInitRef NameService=$ns_endpoint/NameService";
$common_args = "$ns_initref -ORBSvcConf server.conf";

$server_args = "-hm_ior file://$hmior -proc_id local-A -app_id Worker -role 1 -load 15 -p $iorfile -b 45 -i 20 $common_args";
$client_args = "-p file://$iorfile -s Worker -r 500 -e 200 -i 20 $common_args";

$NS = new PerlACE::Process ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
			    "-ORBEndpoint $ns_endpoint -o $nsior");

$RM = new PerlACE::Process ("$ENV{TAO_ROOT}/orbsvcs/LWFT_Service/ReplicationManager",
                            "-use_ns $ns_initref");

$HM = new PerlACE::Process ("$ENV{TAO_ROOT}/orbsvcs/LWFT_Service/HostMonitor",
			    "-load_mon_freq 10 -rm_update_freq 1 -ior_file $hmior -util_file $hmutil -rm_name ReplicationManager $ns_initref");

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "$server_args");
}
else {
    $SV = new PerlACE::Process ("server", "$server_args");
}

$CL = new PerlACE::Process ("client", $client_args);

print "starting Naming_Service...\n";
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1)
{
    print STDERR "ERROR: could not find Naming_Service ior.\n";
    $NS->Kill ();
    exit 1;
}

print "starting ReplicationManager..\n";
$RM->Spawn ();


if (PerlACE::waitforfile_timed ($rmior, $PerlACE::wait_interval_for_process_creation) == -1)
{
    print STDERR "ERROR: could not find ReplicationManager ior.\n";
    $NS->Kill ();
    exit 1;
}


print "starting HostMonitor...\n";
$HM->Spawn ();

if (PerlACE::waitforfile_timed ($hmior, $PerlACE::wait_interval_for_process_creation) == -1)
{
    print STDERR "ERROR: could not find HostMonitor ior.\n";
    $NS->Kill ();
    $RM->Kill ();
    $HM->Kill ();
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1)
{
    $server = $SV->TimedWait (1);
    if ($server == 2)
    {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else
    {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0)
{
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0)
{
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $nsior;
unlink $rmior;
unlink $hmior;
unlink $hmutil;

exit $status;
