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

$TARGETHOSTNAME = $ns->{'HOSTNAME'};
$def_port = 2809;

$nsiorfile = "ns.ior";
$msfile = "MessengerServer.ready";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $srv_msfile = $srv->LocalFile ($msfile);
$ns->DeleteFile ($nsiorfile);
$srv->DeleteFile ($msfile);

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
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

# start the server
print "Start Messenger Server \n";
$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv_msfile ".
                                               "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:$def_port/NameService");
$SRV->Spawn();
if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($srv->WaitForFileTimed ($msfile,$srv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv_msfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

@corbaname_clients = ("corbaname:iiop:$TARGETHOSTNAME:2809#example/Messenger",
                      "corbaname::$TARGETHOSTNAME:2809#example/Messenger",
                      "corbaname::$TARGETHOSTNAME#example/Messenger",
                      "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:2809/NameService corbaname:rir:#example/Messenger");

@clients_comments = ( "Using a corbaname that does not require -ORBDefaultInitRef: \n",
                      "Using defaults to iiop protocol: \n",
                      "Using default protocol (iiop) and default port ($def_port): \n",
                      "Using Resolve Initial Reference form(must tell ORB where NS is located with -ORBInitRef)\n");

# Run the client for each of the corbaname test.
$test_number = 0;
$MessengerClient = "MessengerClient";
foreach $o (@corbaname_clients) {

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
    print "Finish Test $test_number: $clients_comments[$test_number]\n";
    print "             $MessengerClient $o\n";
    print "======================================\n\n";
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

$ns->DeleteFile ($nsiorfile);
$srv->DeleteFile ($msfile);

exit $status;
