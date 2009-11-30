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
use Cwd;

## Save the starting directory
$startdir = getcwd();

$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = 10001 + $client->RandomPort(); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + $server->RandomPort();

$iorfile = "ns.ior";
$persistent_ior_file = "pns.ior";
$persistent_log_file = "test_log";
$data_file = "test_run.data";

my $server_iorfile = $server->LocalFile ($iorfile);
my $client_iorfile = $client->LocalFile ($iorfile);
$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);

$status = 0;

sub name_server
{
    my $args = "-ORBMulticastDiscoveryEndpoint 224.9.9.2:$server_ns_multicast_port -o $server_iorfile -m 1 @_";
    my $prog = "$startdir/../../Naming_Service/Naming_Service";
    
    $SV = $server->CreateProcess ("$prog", "$args");

    $server->DeleteFile($server_iorfile);

    $SV->Spawn ();

    if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    if ($server->GetFile ($iorfile) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($client->PutFile ($iorfile) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

sub client
{
    my $args = "@_"." ";
    my $prog = "$startdir/client";

    $CL = $client->CreateProcess ("$prog", "$args");

    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
    
}

## The options below have been reordered due to a
## initialization problem (within the Naming_Service)
## that has only been seen on Windows XP.

# Options for all simple tests recognized by the 'client' program.
@opts = ("-s -ORBInitRef NameService=file://$client_iorfile",
         "-p $persistent_ior_file -ORBInitRef NameService=file://$client_iorfile",
         "-s -ORBInitRef NameService=mcast://224.9.9.2:$client_ns_multicast_port\::/NameService",
         "-t -ORBInitRef NameService=file://$client_iorfile",
         "-i -ORBInitRef NameService=file://$client_iorfile",
         "-e -ORBInitRef NameService=file://$client_iorfile",
         "-y -ORBInitRef NameService=file://$client_iorfile",
         "-c file://$client_persistent_ior_file -ORBInitRef NameService=file://$client_iorfile",
         );

$hostname = $server->HostName ();

@server_opts = ("-t 30",
                "-ORBEndpoint iiop://$hostname:$server_ns_orb_port -f $server_persistent_log_file",
                "", "", "", "", "",
                "-ORBEndpoint iiop://$hostname:$server_ns_orb_port -f $server_persistent_log_file",
                );

@comments = ("Simple Test: \n",
             "mmap() Persistent Test (Part 1): \n",
             "Simple Test (using multicast to locate the server): \n",
             "Tree Test: \n",
             "Iterator Test: \n",
             "Exceptions Test: \n",
             "Destroy Test: \n",
             "mmap() Persistent Test (Part 2): \n",
             );

$test_number = 0;

## Allow the user to determine where the persistent file will be located
## just in case the current directory is not suitable for locking.
## We can't change the name of the persistent file because that is not
## sufficient to work around locking problems for Tru64 when the current
## directory is NFS mounted from a system that does not properly support
## locking.
foreach my $possible ($ENV{TMPDIR}, $ENV{TEMP}, $ENV{TMP}) {
  if (defined $possible && -d $possible) {
    if (chdir($possible)) {
      last;
    }
  }
}

print "INFO: Running the test in ", getcwd(), "\n";

unlink ($persistent_ior_file, $persistent_log_file);

# Run server and client for each of the tests.  Client uses ior in a
# file to bootstrap to the server.
foreach $o (@opts) {
    name_server ($server_opts[$test_number]);

    print STDERR "\n          ".$comments[$test_number];

    client ($o);

    $SV->Kill ();

    ## For some reason, only on Windows XP, we need to
    ## wait before starting another Naming_Service when
    ## the mmap persistence option is used
    if ($^O eq "MSWin32") {
      sleep(1);
    }

    $test_number++;
}

unlink ($persistent_ior_file, $persistent_log_file);

# Now run the multithreaded test, sending output to the file.
print STDERR "\n          Multithreaded Test:\n";
unlink $data_file;

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">$data_file") or die "can't redirect stdout: $!";
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

# just here to quiet warnings
$fh = \*OLDOUT;
$fh = \*OLDERR;

name_server ();

client ("-ORBInitRef NameService=file://$client_iorfile", "-m15");

close (STDERR);
close (STDOUT);
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

$SV->Kill ();

unlink $iorfile;


$errors = system ("perl $startdir/process-m-output.pl $data_file 15") >> 8;

if ($errors > 0) {
    $status = 1;

    if (!$quiet) {
        print STDERR "Errors Detected, printing output\n";
        if (open (DATA, "<$data_file")) {
            print STDERR "================================= Begin\n";
            print STDERR <DATA>;
            print STDERR "================================= End\n";
            close (DATA);
        }
        else {
            print STDERR "ERROR: Could not open $data_file\n";
        }
        unlink $data_file;
    }
}

unlink $iorfile;

exit $status;
