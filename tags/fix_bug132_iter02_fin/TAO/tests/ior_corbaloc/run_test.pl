eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl Script that runs the corbaloc: style URL tests.
# It starts the servers and client.

use lib "../../../bin";
use PerlACE::Run_Test;

$status = 0;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 8;

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = 10001 + PerlACE::uniqueid (); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + PerlACE::uniqueid ();
$iorfile = PerlACE::LocalFile ("ns.ior");

# Options for all simple tests recognized by the 'client' program.
@opts = ("corbaloc:rir:/NameService",
         "corbaloc:rir:/",
         "corbaloc:iiop://$TARGETHOSTNAME:$ns_orb_port/NameService",
         "corbaloc:iiop://:$ns_orb_port/NameService",
         "corbaloc:///NameService",
         "corbaloc://$TARGETHOSTNAME:$ns_orb_port/NameService");


$def_port = 2809;

@name_server_opts = ("-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
                     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
                     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
                     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port",
                     "-ORBEndPoint iiop://$TARGETHOSTNAME:$def_port",
                     "-ORBEndPoint iiop://$TARGETHOSTNAME:$ns_orb_port");


@comments = ("Using RIR protocol with the Key object specified: \n",
             "Using RIR protocol without specifying the key object: \n",
             "Using IIOP protocol with the hostname, port specified: \n",
             "Using IIOP protocol with the hostname unspecified but port specified: \n",
             "Using IIOP protocol with both hostname & port unspecified: \n",
             "Using the default protocol (iiop) with the hostname, port specified: \n");

$test_number = 0;

# Run the naming service, server. Run the client for each of the test. 
# Client uses ior in a file to bootstrap to the server.

$SR = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client");
$NS = new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service");

foreach $o (@opts) {  
    # Make sure the files are gone, so we can wait on them.
    unlink $iorfile;

    $NS->Arguments ($name_server_opts [$test_number]
                    . " -ORBnameserviceport $ns_multicast_port -o $iorfile");
    
    $NS->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
        print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
        $NS->Kill (); 
        exit 1;
    }

    print STDERR "\n          ".$comments[$test_number];

    $SR->Arguments ("ORBInitRef NameService=file://$iorfile");
    $SR->Spawn ();
    
    sleep $sleeptime;

    $CL->Arguments ("$o -ORBInitRef NameService=file://$iorfile");
    $client = $CL->SpawnWaitKill (60);
    
    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
    
    $nameservice = $NS->TerminateWaitKill (10); 

    if ($nameservice != 0) {
        print STDERR "ERROR: name server returned $nameservice\n";
        $status = 1;
    }

    $server = $SR->TerminateWaitKill (10);
    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    $test_number++;
}

exit $status;
