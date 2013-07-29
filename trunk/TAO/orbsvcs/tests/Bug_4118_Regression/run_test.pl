eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

## Save the starting directory
$status = 0;
$startdir = getcwd();

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# Variables for command-line arguments to client and server
# executables.
$ns_orb_port = 12000 + $test->RandomPort();

$iorfile = "ns.ior";
$persistence_dir = "NameService";

#Files which used by test
my $test_iorfile = $test->LocalFile ($iorfile);

# clean up from a previous test run, or from this run
sub cleanup
{
    $test->DeleteFile($iorfile);
    if ( ! -d $persistence_dir ) {
        mkdir ($persistence_dir, 0777);
    }
    else {
        chdir $persistence_dir;
        opendir(THISDIR, ".");
        @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
        closedir(THISDIR);
        foreach $tmp (@allfiles){
            $test->DeleteFile ($tmp);
        }
        chdir "..";
    }

}

sub start_name_server
{
    my $args = "-ORBCollocation no -ORBListenEndpoints iiop://$hostname:$ns_orb_port -o $test_iorfile -n 5 -u $persistence_dir";
    my $prog = "$startdir/../../Naming_Service/tao_cosnaming";

    $SV = $test->CreateProcess ("$prog", "$args");
    $SV->Spawn ();

    if ($test->WaitForFileTimed ($iorfile,
                               $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

sub nsadd
{
    my $args = "--ns file://$test_iorfile "."@_"." ";
    my $prog = "$startdir/../../../utils/nslist/tao_nsadd";

    $CL = $test->CreateProcess ("$prog", "$args");

    $client_status = $CL->SpawnWaitKill ($test->ProcessStartWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: ns_add returned $client_status\n";
        $status = 1;
    }

}

#1. use the tao_nsadd command add a cyclical reference to the root context
#2. use the tao_nsadd command to add a binding that traverses the cycle
#3. verify that step 2 does not hang
#4. clean up old references

@opts = ("--name recurse --ior file://$test_iorfile --ctx",
         "--name recurse/recurse/recurse/dummy --ior file://$test_iorfile");

$hostname = $test->HostName ();

@comments = ("Create naming context cycle: \n",
             "bind reference through cycle:  \n",
             );

$test_number = 0;

print "INFO: Running the test in ", getcwd(), "\n";

cleanup ();
start_name_server ();

foreach $o (@opts) {

    print STDERR "$comments[$test_number]\n";

    nsadd ($o);

    $test_number++;
}

$SV->Kill ();

cleanup ();

rmdir ($persistence_dir);

exit $status;
