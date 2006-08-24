eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 10;

# The default host name.
$TARGETHOSTNAME = "localhost";

$quiet = 0;
$use_ssl = 1;
$debug_level = 0;

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
$ns_multicast_port = 10001 + PerlACE::uniqueid (); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + PerlACE::uniqueid ();
$ns_ssl_port = 14000 + PerlACE::uniqueid ();
$iorfile = PerlACE::LocalFile ("ns.ior");
$persistent_ior_file = PerlACE::LocalFile ("pns.ior");
$persistent_log_file = PerlACE::LocalFile ("test_log");
$data_file = PerlACE::LocalFile ("test_run.data");

$status = 0;

sub name_server
{
    my $args = "-ORBNameServicePort $ns_multicast_port -o $iorfile -m 1 @_";
    my $prog = "../../../Naming_Service/Naming_Service";
    $NS = new PerlACE::Process ($prog, $args);

    unlink $iorfile;

    print STDERR "CommandLine: server " . $NS->Arguments() . "\n\n" unless $quiet;
    $ns_server = $NS->Spawn ();

    if ($ns_server != 0) {
        print STDERR "ERROR: NS returned $ns_server\n";
        exit 1;
    }

    if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
        print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
        $NS->Kill (); 
        exit 1;
    }
}

sub client
{
    my $args = "@_"." ";
    my $prog = "client";

    $CL = new PerlACE::Process ($prog, $args);

    print STDERR "CommandLine: client " . $CL->Arguments() . "\n\n" unless $quiet;
    $client = $CL->SpawnWaitKill (60);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
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
                . " -ORBSvcConf client.conf $orb_debug_level",
         "-t -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf client.conf $orb_debug_level",
         "-i -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf client.conf $orb_debug_level",
         "-e -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf client.conf $orb_debug_level",
         "-y -ORBInitRef NameService=corbaloc:ssliop:$TARGETHOSTNAME:$ns_ssl_port/NameService"
                . " -ORBSvcConf client.conf $orb_debug_level",
         
         );

@server_opts = (
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf server.conf $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port"
                        . " -ORBSvcConf server.conf $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port" 
                        . " -ORBSvcConf server.conf $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port" 
                        . " -ORBSvcConf server.conf $orb_debug_level",
                "-ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port/ssl_port=$ns_ssl_port" 
                        . " -ORBSvcConf server.conf $orb_debug_level",
                
                );

@comments = (
             "(SSL) Simple Test:",
             "(SSL) Tree Test:",
             "(SSL) Iterator Test:",
             "(SSL) Exceptions Test:",
             "(SSL) Destroy Test:",
             );

$test_number = 0;

unlink ($persistent_ior_file, $persistent_log_file);

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

    $NS->Kill ();

    ## For some reason, only on Windows XP, we need to
    ## wait before starting another Naming_Service when
    ## the mmap persistence option is used
    if ($^O eq "MSWin32") {
      sleep(1);
    }

    $test_number++;
}

unlink ($persistent_ior_file, $persistent_log_file);
unlink $iorfile;

exit $status;
