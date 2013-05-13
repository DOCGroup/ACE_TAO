eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs additional Naming Service tests.
# It runs all the tests that will *not* run with min CORBA.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

## Save the starting directory
$status = 0;
$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = 10001 + $test->RandomPort(); # Can not be 10000 on Chorus 4.0
$ns_orb_port = 12000 + $test->RandomPort();

$iorfile = "ns.ior";
$persistent_ior_file = "fpns.ior";


$iorfile = "ns.ior";
$persistent_ior_file = "pns.ior";

my $test_iorfile = $test->LocalFile ($iorfile);
my $test_persistent_ior_file = $test->LocalFile ($persistent_ior_file);
my $prog = "../../Naming_Service/tao_cosnaming";

$test->DeleteFile($iorfile);
$test->DeleteFile($persistent_ior_file);

sub name_server
{
    my $args = "-ORBNameServicePort $ns_multicast_port -o $test_iorfile -m 1 @_";


    $SV = $test->CreateProcess ("$prog", "$args");

    $test->DeleteFile($iorfile);

    $SV->Spawn ();

    if ($test->WaitForFileTimed ($iorfile,
                               $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

sub client
{
    my $args = "@_"." ";
    my $prog = "client";

    $CL = $test->CreateProcess ("$prog", "$args");

    $client_status = $CL->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
}

## The options below have been reordered due to a
## initialization problem (within the Naming_Service)
## that has only been seen on Windows XP.

# Options for all simple tests recognized by the 'client' program.
@opts = ("-p $test_persistent_ior_file -ORBInitRef NameService=file://$test_iorfile",
         "-c file://$test_persistent_ior_file -ORBInitRef NameService=file://$test_iorfile",
         "-l file://$test_persistent_ior_file -ORBInitRef NameService=file://$test_iorfile");

$hostname = $test->HostName ();

@server_opts = ("-ORBEndpoint iiop://$hostname:$ns_orb_port -u NameService",
                "-ORBEndpoint iiop://$hostname:$ns_orb_port -u NameService",
                "-ORBEndpoint iiop://$hostname:$ns_orb_port -u NameService"
                );

@comments = ("Flat File Persistent Test (Part 1): \n",
             "Flat File Persistent Test (Part 2): \n",
             "Flat File Persistent Test (Part 3): \n");


sub run_test
{
    $prog = "@_";

    $test_number = 0;

    $test->DeleteFile($test_persistent_ior_file);

    if ( ! -d "NameService" ) {
        mkdir (NameService, 0777);
    }
    else {
        chdir "NameService";
        opendir(THISDIR, ".");
        @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
        closedir(THISDIR);
        foreach $tmp (@allfiles){
            $test->DeleteFile ($tmp);
        }
        chdir "..";
    }

    # Run server and client for each of the tests.  Client uses ior in a
    # file to bootstrap to the server.
    foreach $o (@opts) {
        name_server ($server_opts[$test_number]);

        print STDERR "\n          ".$comments[$test_number];

        client ($o);

        $SV->Kill ();

        ## For some reason, only on Windows XP, we need to
        ## wait before starting another tao_cosnaming when
        ## the mmap persistence option is used
        if ($^O eq "MSWin32") {
          sleep(1);
        }

        $test_number++;
    }

    chdir "NameService";
    opendir(THISDIR, ".");
    @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
    closedir(THISDIR);
    foreach $tmp (@allfiles){
        $test->DeleteFile ($tmp);
    }
    chdir "..";
    rmdir "NameService";

    $test->DeleteFile($persistent_ior_file);
    $test->DeleteFile($iorfile);
}

@server_exes = ("../../Naming_Service/tao_cosnaming",
                "../../FT_Naming_Service/tao_ft_naming");

foreach $e (@server_exes) {
    print STDERR "Testing Naming Service Executable: $e\n";
    run_test($e);
    print STDERR "======================================\n";
}

exit $status;
