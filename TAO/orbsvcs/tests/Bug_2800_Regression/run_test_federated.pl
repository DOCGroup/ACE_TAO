eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;

$quiet = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($ARGV[0] eq '-q') {
        $quiet = 1;
    }
}


# Variables for command-line arguments to client and server
# executables.
$ns_orb_port1 = 9931;
$ns_orb_port2 = 9932;
$ns_endpoint1 = "iiop://localhost:$ns_orb_port1";
$ns_endpoint2 = "iiop://localhost:$ns_orb_port2";
$nsiorfile1 = PerlACE::LocalFile ("ns1.ior");
$nsiorfile2 = PerlACE::LocalFile ("ns2.ior");
$iorfile = PerlACE::LocalFile ("test.ior");

$status = 0;

# Run two Naming Servers

my $args = "-ORBEndPoint $ns_endpoint1 -o $nsiorfile1 -m 0";
my $prog = "../../Naming_Service/Naming_Service";
$NS1 = new PerlACE::Process ($prog, $args);

unlink $nsiorfile1;
unlink $iorfile;

$NS1->Spawn ();

if (PerlACE::waitforfile_timed ($nsiorfile1, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: cannot find IOR file <$nsiorfile1>\n";
  $NS1->Kill ();
  exit 1;
}

my $args = "-ORBEndPoint $ns_endpoint2 -o $nsiorfile2 -m 0";
my $prog = "../../Naming_Service/Naming_Service";
$NS2 = new PerlACE::Process ($prog, $args);

unlink $nsiorfile2;

$NS2->Spawn ();

if (PerlACE::waitforfile_timed ($nsiorfile2, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: cannot find IOR file <$nsiorfile2>\n";
  $NS2->Kill ();
  exit 1;
}

$SV = new PerlACE::Process ("server", "-ORBDebuglevel $debug_level");
$CL = new PerlACE::Process ("client", "");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

# Kill naming service 2
print STDERR "Killing second naming server\n";
$NS2->Kill ();

print STDERR "Starting client\n";
$client = $CL->Spawn ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

sleep (15);

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$NS1->Kill ();
$CL->TerminateWaitKill (2);

unlink $iorfile1;
unlink $iorfile2;

exit $status;
