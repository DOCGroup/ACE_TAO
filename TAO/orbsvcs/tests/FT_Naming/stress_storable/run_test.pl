eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

$status = 0;
$debug_level = 0;

$startdir = getcwd();

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";



# Variables for command-line arguments to naming service executables.
$hostname = $test->HostName ();

$ns_orb_port1 = 10001 + $test->RandomPort ();
$ns_orb_port2 = 10002 + $test->RandomPort ();
$ns_endpoint1 = "iiop://$hostname:$ns_orb_port1";
$ns_endpoint2 = "iiop://$hostname:$ns_orb_port2";

$ns_threads = 10; #was 50
$hammers = 15;    #was 60, but that seems a little much for nightly build test.
$persist_root = ".";

if ($#ARGV >= 0) {
    my $sn_set = 0;
    for (my $i = 0; $i <= $#ARGV; $i++) {
        if ($ARGV[$i] eq '-debug') {
            $debug_level = 10;
        }
        elsif ($ARGV[$i] eq "-n") {
            $i++;
            $ns_threads = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-h") {
            $i++;
            $hammers = $ARGV[$i];
        }
        elsif ($ARGV[$i] eq "-p") {
            $i++;
            $persist_root = $ARGV[$i];
        }
    }
}


$naming_persistence_dir = "$persist_root/NameService";
$groups_persistence_dir = "$persist_root/Groups";

# $primary_iorfile = "$naming_persistence_dir/ns_replica_primary.ior";
$ns_iorfile = "ns.ior";
$nm_iorfile = "nm.ior";
$primary_logfile = "ns.log";

my $test_combined_ns_iorfile = $test->LocalFile ($ns_iorfile);
my $test_combined_nm_iorfile = $test->LocalFile ($nm_iorfile);
my $test_primary_iorfile = $test->LocalFile ($primary_iorfile);
my $test_primary_iorfile = $test->LocalFile ($primary_logfile);

my $server_obj_group_iorfile = "obj.ior";

#Files used by the server2
my $server2_nm_iorfile = $server2->LocalFile ($nm_iorfile);
$server2->DeleteFile($server2_nm_iorfile);
my $server2_ns_iorfile = $server2->LocalFile ($ns_iorfile);
$server2->DeleteFile($server2_ns_iorfile);
my $server2_obj_group_iorfile = $server2->LocalFile ($server_obj_group_iorfile);
$server2->DeleteFile($server_obj_group_iorfile);

#Files which used by client
my $client_ns_iorfile = $client->LocalFile ($ns_iorfile);
my $client_nm_iorfile = $client->LocalFile ($nm_iorfile);

$status = 0;

print "INFO: Running the test in ", getcwd(), "\n";


sub clean_persistence_dir($$)
{
    my $target = shift;
    my $directory_name = shift;

    chdir $directory_name;
    opendir(THISDIR, ".");
    @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
    closedir(THISDIR);
    foreach $tmp (@allfiles){
        $target->DeleteFile ($tmp);
    }
    chdir $startdir;
}

# Make sure that the directory to use to hold the persistence data
# exists and is cleaned out.
sub init_persistence_directory($$)
{
    my $target = shift;
    my $directory_name = shift;

    if ( ! -d $directory_name ) {
        mkdir ($directory_name, 0777);
    } else {
        clean_persistence_dir ($target, $directory_name);
    }
}

my $args = "-ORBEndPoint $ns_endpoint1 " .
    "-m 0 " .
    "-u $naming_persistence_dir " .
    "-v $groups_persistence_dir " .
    "-n $ns_threads " .
    "-o $ns_iorfile -h $nm_iorfile " .
    "-ORBDebuglevel $debug_level -ORBVerboseLogging 1 -ORBLogFile $primary_logfile ";

my $prog = "$startdir/../../../FT_Naming_Service/tao_ft_naming";

print STDERR "Starting Primary: $prog $args\n";

$NS1 = $test->CreateProcess ("$prog", "$args");

# Clean out the primary ior file to allow us to wait for it
$test->DeleteFile ($primary_iorfile);
$test->DeleteFile ($primary_logfile);
init_persistence_directory ($test, $naming_persistence_dir);
init_persistence_directory ($test, $groups_persistence_dir);

$NS1->Spawn ();

if ($test->WaitForFileTimed ($ns_iorfile,
                             $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_primary_iorfile>\n";
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

$SV2 = $server2->CreateProcess ("server",
                              "-ORBInitRef NameService=file://$server2_ns_iorfile " .
                              "-ORBInitRef NamingManager=file://$server2_nm_iorfile " .
                              "-o $server2_obj_group_iorfile");

$CL = $client->CreateProcess ("client",
                              "-n $hammers " .
                              "-ORBInitRef NameService=file://$client_ns_iorfile " .
                              "-ORBInitRef NameManager=file://$client_nm_iorfile ");

print STDERR "\n\n======== Running the Storable Stress Test================\n";

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
}

if ($server2->WaitForFileTimed ($server_obj_group_iorfile,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_obj_group_iorfile>\n";
    KillServers ();
    exit 1;
}

if ($server2->GetFile ($server_obj_group_iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_obj_group_iorfile>\n";
    KillServers ();
    exit 1;
}

if ($client->PutFile ($server_obj_group_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_ns_iorfile>\n";
    KillServers ();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$NS1->Kill ();


exit $status;
