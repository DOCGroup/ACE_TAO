# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
  &  eval 'exec perl -S $0 $argv:q'
  if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $srv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cli = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $tnd = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$TARGETHOSTNAME = $ns->HostName ();
$def_port = 2809;

my $nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = $ns->CreateProcess ($NameService, " -ORBListenEndpoints iiop://$TARGETHOSTNAME:$def_port ".
                                        "-o $ns_nsiorfile");
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# File is only used to make sure the NS is fully running, so we can
# remove it now.
$ns->DeleteFile ($nsiorfile);

# List of tests to test corbaloc URL syntax.
@corbaloc_servers = ( "-ORBDefaultInitRef corbaloc::$TARGETHOSTNAME",
                      "-ORBInitRef NameService=corbaloc::$TARGETHOSTNAME/NameService",
                      "-ORBInitRef NameService=corbaloc::$TARGETHOSTNAME:$def_port/NameService");


@servers_comments = ( "Using -ORBDefaultInitRef with default port $def_port and default transport IIOP: \n",
                      "Using -ORBInitRef with corbaloc URL: \n",
                      "Using -ORBInitRef with corbaloc URL and specified port: \n");

@corbaloc_clients = ( "corbaloc::$TARGETHOSTNAME/NameService",
                      "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:$def_port/NameService corbaloc:rir:/NameService");

@clients_comments = ( "Using the URL parameter: \n",
                      "Using a corbaloc:rir form URL(must specify initial reference): \n");

$MessengerServer= "MessengerServer";
$MessengerClient= "MessengerClient";
$nsdel = "$ENV{ACE_ROOT}/bin/tao_nsdel";

$test_number = 0;

foreach $o (@corbaloc_servers) {
    # Run messenger server for each test.
    #print "Start $MessengerServer $o \n";
    $SRV = $srv->CreateProcess ($MessengerServer, "-ORBdebuglevel $debug_level ".
                                               "$o");
    $SRV->Spawn();
    if ($SRV_status != 0) {
        print STDERR "ERROR: Messenger Server returned $SRV_status\n";
        exit 1;
    }

    #print "Start $MessengerClient \n";
    $CLI = $cli->CreateProcess ($MessengerClient, "-ORBDefaultInitRef iiop://$TARGETHOSTNAME:$def_port");
    $test_number++;

    $CLI_status = $CLI->SpawnWaitKill($cli->ProcessStartWaitInterval());
    if ($CLI_status != 0) {
        print STDERR "ERROR: Messenger Client returned $CLI_status\n";
        $SRV->Kill(); $SRV->TimedWait (1);
        $NS->Kill(); $NS->TimedWait (1);
        exit 1;
    }

    print "======================================\n";
    print "Finish Test $test_number: $servers_comments[$test_number] \n";
    print "  $MessengerServer $o\n";
    print "======================================\n\n";

    $SRV->Kill(); $SRV->TimedWait (1);

    # remove ns entry...
    $TND = $tnd->CreateProcess ($nsdel, "--quiet ".
                                        "--name  example/Messenger ".
                                        "-ORBInitRef NameService=corbaloc::$TARGETHOSTNAME:$def_port/NameService");
    $TND_status = $TND->SpawnWaitKill($tnd->ProcessStartWaitInterval());
    if ($TND_status != 0) {
        print STDERR "ERROR: tao_nsdel failed with status $TND_status\n";
        $NS->Kill(); $NS->TimedWait (1);
        exit 1;
    }
}


#print "Start $MessengerServer \n";
$SRV = $srv->CreateProcess ($MessengerServer, "-ORBdebuglevel $debug_level ".
                                              "-ORBDefaultInitRef iiop://$TARGETHOSTNAME:$def_port");
$SRV->Spawn();
if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    exit 1;
}

$i = 0;
foreach $o (@corbaloc_clients) {

    # Run the client for each test.
    #print "Start $MessengerClient $o \n";
    $CLI = $cli->CreateProcess ($MessengerClient, "$o");

    $CLI_status = $CLI->SpawnWaitKill($cli->ProcessStartWaitInterval());
    if ($CLI_status != 0) {
        print STDERR "ERROR: Messenger Client returned $CLI_status\n";
        $SRV->Kill(); $SRV->TimedWait (1);
        $NS->Kill(); $NS->TimedWait (1);
        exit 1;
    }

    $test_number++;
    print "======================================\n";
    print "Finish Test $test_number: $clients_comments[$i]\n";
    print "             $MessengerClient $o";
    print "\n======================================\n\n";

    $i ++;
}


# clean up
$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

exit $status;
