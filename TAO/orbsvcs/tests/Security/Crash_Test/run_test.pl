eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

$server_conf = PerlACE::LocalFile ("server$PerlACE::svcconf_ext");
$client_conf = PerlACE::LocalFile ("client$PerlACE::svcconf_ext");

$server_startup_wait_time = 5; 
$client_wait_time         = 15;

$client_args = "-ORBSvcConf $client_conf -w $client_wait_time -k file://$iorfile";

# Set the SSL environment
$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = new PerlACE::Process ("server",
			    "-o $iorfile " .
			    "-ORBSvcConf $server_conf " .
			    "-ORBdotteddecimaladdresses 1 " .
			    "-ORBEndpoint iiop://:/ssl_port=1235");

$CL1 = new PerlACE::Process ("client", "-s $client_args");
$CL2 = new PerlACE::Process ("client", "$client_args");
$CL3 = new PerlACE::Process ("client", "$client_args");


local $start_time = time();
local $max_running_time = 500; # < 10 minutes
local $elapsed = time() - $start_time;

while($status == 0 && $elapsed < $max_running_time)
{
    # Start the server
    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $server_startup_wait_time) == -1)
    {
	print STDERR "ERROR: cannot find file <$iorfile>\n";
	$SV->Kill (); $SV->TimedWait (1);
	exit 1;
    }

    # Start all clients in parallel
    $CL1->Spawn ();
    $CL2->Spawn ();
    $CL3->Spawn ();

    # Let our clients to execute few requests
    sleep (3);

    # Now kill the server and start it again.
    $SV->Kill ();
    $SV->TimedWait (5);
    
    unlink $iorfile;
    
    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $server_startup_wait_time) == -1)
    {
	print STDERR "ERROR: cannot find file <$iorfile>\n";
	$SV->Kill (); $SV->TimedWait (1);
	exit 1;
    }

    # Wait for the server and clients to finish

    # Waiting for the first client longer because it waits for others and shutdowns the server.
    $client1 = $CL1->WaitKill ($client_wait_time * 2); 
    $client2 = $CL2->WaitKill ($client_wait_time * 2);
    $client3 = $CL3->WaitKill ($client_wait_time * 2);
    $server = $SV->WaitKill (5); # should be down already

    unlink $iorfile;

    if ($client1)
    {
	print STDERR "ERROR: client #1 returned $client1\n";
	$status = 1;
    }

    if ($client2)
    {
	print STDERR "ERROR: client #2 returned $client2\n";
	$status = 1;
    }

    if ($client3)
    {
	print STDERR "ERROR: client #3 returned $client3\n";
	$status = 1;
    }

    if ($server)
    {
	print STDERR "ERROR: server returned $server\n";
	$status = 1;
    }

    $elapsed = time() - $start_time;
}

exit $status;
