eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests
# for the FT_Naming_Service.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

## Save the starting directory
$status = 0;
$multicast = '224.9.9.2';
$startdir = getcwd();

$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# Variables for command-line arguments to client and server
# executables.
$ns_multicast_port = 10001 + $test->RandomPort(); # Can not be 10000 on Chorus 4.0

$iorfile = "ns.ior";
$persistent_ior_file = "pns.ior";
$persistent_log_file = "test_log";

$data_file = "test_run.data";

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

$test_log = $test->LocalFile ($data_file);
$test->DeleteFile ($data_file);

#Files which used by test
my $test_iorfile = $test->LocalFile ($iorfile);
my $test_persistent_log_file = $test->LocalFile ($persistent_log_file);
my $test_persistent_ior_file = $test->LocalFile ($persistent_ior_file);

$test->DeleteFile($persistent_ior_file);
$test->DeleteFile($iorfile);
$test->DeleteFile($persistent_log_file);

sub name_server
{
    my $args = "-u NameService -ORBMulticastDiscoveryEndpoint $multicast:$ns_multicast_port -o $test_iorfile -m 1 @_";
    my $prog = "$startdir/../../Naming_Service/tao_ft_naming";

    $SV = $test->CreateProcess ("$prog", "$args");

    $test->DeleteFile($iorfile);

    $SV->Spawn ();

    if ($test->WaitForFileTimed ($iorfile,
                               $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    sleep(10);
}

sub client
{
    my $args = "@_"." ";
    my $prog = "$startdir/client";

    $CL = $test->CreateProcess ("$prog", "$args");

    $client_status = $CL->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

}

sub make_or_clean_state
{
    # Create a directory to hold the persistent state
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
}

## The options below have been reordered due to a
## initialization problem (within the Naming_Service)
## that has only been seen on Windows XP.

# Options for all simple tests recognized by the 'client' program.
@opts = ("-s -ORBInitRef NameService=file://$test_iorfile",
         "-s -ORBInitRef NameService=mcast://$multicast:$ns_multicast_port\::/NameService",
         "-t -ORBInitRef NameService=file://$test_iorfile",
         "-i -ORBInitRef NameService=file://$test_iorfile",
         "-e -ORBInitRef NameService=file://$test_iorfile",
         "-y -ORBInitRef NameService=file://$test_iorfile"
         );

@server_opts = ("-t 30",
                "",
                "",
                "",
                "",
                ""
                );

@comments = ("Simple Test: \n",
             "Simple Test (using multicast to locate the server): \n",
             "Tree Test: \n",
             "Iterator Test: \n",
             "Exceptions Test: \n",
             "Destroy Test: \n"
             );

$test_number = 0;

print "INFO: Running the test in ", getcwd(), "\n";

# Run server and client for each of the tests.  Client uses ior in a
# file to bootstrap to the server.
foreach $o (@opts) {

    # Ensure that the name service is starting clean each time
    make_or_clean_state;

    print STDERR "Running Test: $comments[$test_number]\n";

    name_server ($server_opts[$test_number]);

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

$test->DeleteFile($persistent_ior_file);
$test->DeleteFile($persistent_log_file);
$test->DeleteFile($iorfile);

# Now run the multithreaded test, sending output to the file.
print STDERR "\n          Multithreaded Test:\n";
$test->DeleteFile ($data_file);

name_server ();
client ("-ORBInitRef NameService=file://$test_iorfile -ORBLogFile $test_log", "-m15");

$SV->Kill ();

$errors = system ("perl $startdir/process-m-output.pl $test_log 15") >> 8;

if ($errors > 0) {
    $status = 1;

    if (!$quiet) {
        print STDERR "Errors Detected, printing output\n";
        if (open (DATA, "<$test_log")) {
            print STDERR "================================= Begin\n";
            print STDERR <DATA>;
            print STDERR "================================= End\n";
            close (DATA);
        }
        else {
            print STDERR "ERROR: Could not open $test_log\n";
        }
        $test->DeleteFile ($data_file);
    }
}

$test->DeleteFile($iorfile);
# Remove the persistence directory
rmdir "NameService";


exit $status;
