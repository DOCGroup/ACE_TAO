eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

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
my $cli = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

# The default host name.
$TARGETHOSTNAME = $ns->HostName ();;

$quiet = 0;
$use_ssl = 1;

while (my $argument = shift) {
   if ($argument eq '-q') {
       $quiet = 1;
       next;
   }
   if ($argument eq '-d') {
       $debug_level = shift;
       next;
   }
   print STDERR "Unknown: $argument\nUsage $0 [-q] [-d n]\n";
   exit(-1);
}

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = $ns->RandomPort (); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 2000 + $ns->RandomPort ();
$ns_ssl_port = 4000 + $ns->RandomPort ();
$nsiorfile = "ns.ior";
$cliconffile = "client.conf";
$nsconffile = "server.conf";
#$persistent_ior_file = "pns.ior";
#$persistent_log_file = "test_log";
#$data_file = "test_run.data";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $cli_nsiorfile = $cli->LocalFile ($nsiorfile);
my $ns_nsconffile = $ns->LocalFile ($nsconffile);
my $cli_cliconffile = $cli->LocalFile ($cliconffile);
$ns->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);


sub name_server
{
    my $args = "-ORBNameServicePort $ns_multicast_port -o $ns_nsiorfile -m 1 @_";
    my $prog = "../../../Naming_Service/Naming_Service";
    $NS = $ns->CreateProcess ($prog, $args);

    $ns->DeleteFile ($nsiorfile);
    $cli->DeleteFile ($nsiorfile);

    print STDERR "CommandLine: server " . $NS->Arguments() . "\n\n" unless $quiet;
    $NS_status = $NS->Spawn ();

    if ($NS_status != 0) {
        print STDERR "ERROR: Name Service returned $NS_status\n";
        exit 1;
    }
    if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
        print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    if ($ns->GetFile ($nsiorfile) == -1) {
        print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    if ($cli->PutFile ($nsiorfile) == -1) {
        print STDERR "ERROR: cannot set file <$cli_nsiorfile>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
}

sub client
{
    my $args = "@_"." ";
    my $prog = "client";

    $CLI = $cli->CreateProcess ($prog, $args);

    print STDERR "CommandLine: client " . $CLI->Arguments() . "\n\n" unless $quiet;
    $CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval()+45);
    if ($CLI_status != 0) {
        print STDERR "ERROR: client returned $CLI_status\n";
        $status = 1;
    }
}

## The options below have been reordered due to a
## initialization problem (within the Naming_Service)
## that has only been seen on Windows XP.

$orb_debug_level =  ($quiet || $debug_level == 0) ? "" : "-ORBDebugLevel $debug_level";

# Options for all simple tests recognized by the 'client' program.
@opts = (
         "-s -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf $cli_cliconffile $orb_debug_level",
         "-t -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf $cli_cliconffile $orb_debug_level",
         "-i -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf $cli_cliconffile $orb_debug_level",
         "-e -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf $cli_cliconffile $orb_debug_level",
         "-y -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf $cli_cliconffile $orb_debug_level",

         );

@server_opts = (
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf $ns_nsconffile $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf $ns_nsconffile $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf $ns_nsconffile $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf $ns_nsconffile $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf $ns_nsconffile $orb_debug_level",

                );

@comments = (
             "(SSL) Simple Test:",
             "(SSL) Tree Test:",
             "(SSL) Iterator Test:",
             "(SSL) Exceptions Test:",
             "(SSL) Destroy Test:",
             );

$test_number = 0;

# Run server and client for each of the tests.  Client uses ior in a
# file to bootstrap to the server.
foreach $o (@opts) {

    print STDERR "\n          " . $comments[$test_number];
    if ($use_ssl == 0 && $comments[$test_number] =~ /SSL/) {
        print STDERR " - Skipping, use -s to run the SSL tests";
        next;
    }

    print STDERR "\n";
    name_server ($server_opts[$test_number]);
    client ($o);

    $NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
    if ($NS_status != 0) {
        print STDERR "ERROR: Naming Service returned $NS_status\n";
        $status = 1;
        last;
    }

    ## For some reason, only on Windows XP, we need to
    ## wait before starting another Naming_Service when
    ## the mmap persistence option is used
    if ($^O eq "MSWin32") {
      sleep(1);
    }

    $test_number++;
}

$ns->DeleteFile ($nsiorfile);
$cli->DeleteFile ($nsiorfile);

exit $status;
