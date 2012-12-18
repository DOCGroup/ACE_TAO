eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

#$ENV{ACE_TEST_VERBOSE} = "1";

$status = 0;
$debug_level = '0';
$quiet = 0;

if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

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
    chdir "..";
}

# Make sure that the directory to use to hold the naming contexts exists
# and is cleaned out
sub init_naming_context_directory($$)
{
    my $target = shift;
    my $directory_name = shift;

    if ( ! -d $directory_name ) {
        mkdir ($directory_name, 0777);
    } else {
        clean_persistence_dir ($target, $directory_name);
    }
}


# Variables for command-line arguments to client and server
# executables.
my $ns_orb_primary_port1 = 9931;
my $ns_orb_primary_port2 = 9932;
my $ns_orb_backup_port1  = 9933;
my $ns_orb_backup_port2  = 9934;

my $ns_primary_endpoint1 = "iiop://localhost:$ns_orb_primary_port1";
my $ns_backup_endpoint1  = "iiop://localhost:$ns_orb_backup_port1";
my $ns_primary_endpoint2 = "iiop://localhost:$ns_orb_primary_port2";
my $ns_backup_endpoint2  = "iiop://localhost:$ns_orb_backup_port2";

my $ns1_ior = "ns1.ior";
my $ns2_ior = "ns2.ior";
my $nm1_ior = "nm1.ior";
my $nm2_ior = "nm2.ior";
my $iorbase = "test.ior";

my $ns1    = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ns2    = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $server_iorfile = $server->LocalFile ($iorbase);
my $ns1_iorfile    = $ns1->LocalFile ($ns1_ior);
my $ns2_iorfile    = $ns2->LocalFile ($ns2_ior);
my $nm1_iorfile    = $ns1->LocalFile ($nm1_ior);
my $nm2_iorfile    = $ns2->LocalFile ($nm2_ior);

$server->DeleteFile ($iorbase);
$ns1->DeleteFile ($ns1_ior);
$ns2->DeleteFile ($ns2_ior);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $debug_level");

my $tao_ft_naming = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming";

my $name_dir1 = "NameService1";
my $name_dir2 = "NameService2";

init_naming_context_directory ($ns1, $name_dir1);
init_naming_context_directory ($ns2, $name_dir2);

my $ns1_primary_args = "--primary -ORBListenEndPoints $ns_primary_endpoint1 ".
                       "-r $name_dir1";

my $ns1_backup_args = "--backup -ORBListenEndPoints $ns_backup_endpoint1 ".
                      "-c $ns1_iorfile -g $nm1_iorfile ".
                      "-r $name_dir1";

my $ns2_primary_args = "--primary -ORBListenEndPoints $ns_primary_endpoint2 ".
                       "-r $name_dir2";

my $ns2_backup_args = "--backup -ORBListenEndPoints $ns_backup_endpoint2 ".
                      "-c $ns2_iorfile -g $nm2_iorfile ".
                      "-r $name_dir2";

$NS1_PRIMARY = $ns1->CreateProcess($tao_ft_naming, $ns1_primary_args );
$NS1_BACKUP  = $ns1->CreateProcess($tao_ft_naming, $ns1_backup_args );

$NS2_PRIMARY = $ns2->CreateProcess($tao_ft_naming, $ns2_primary_args );
$NS2_BACKUP  = $ns2->CreateProcess($tao_ft_naming, $ns2_backup_args );

my $replica_primary_ior     = "ns_replica_primary.ior";
my $ns1_replica_primary_ior = "$name_dir1/$replica_primary_ior";
my $ns2_replica_primary_ior = "$name_dir2/$replica_primary_ior";

################################################################################
# setup END block to cleanup after exit call
################################################################################
END
{
    $server->DeleteFile ($iorbase);

    $ns1->DeleteFile ($ns1_ior);
    $ns1->DeleteFile ($nm1_ior);

    $ns2->DeleteFile ($ns2_ior);
    $ns2->DeleteFile ($nm2_ior);

    if ( -d $name_dir1 ) {
        print STDERR "INFO: removing <$name_dir1>\n";
        clean_persistence_dir ($ns1, $name_dir1);
        rmdir $name_dir1;
    }

    if ( -d $name_dir2 ) {
        print STDERR "INFO: removing <$name_dir2>\n";
        clean_persistence_dir ($ns2, $name_dir2);
        rmdir $name_dir2;
    }
}

# Run two Naming Servers
$process_status = $NS1_PRIMARY->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: ns1 returned $process_status\n";
    exit 1;
}

if ($ns1->WaitForFileTimed ($ns1_replica_primary_ior,
                            $ns1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$name_dir1/ns_replica_primary.ior>\n";
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

$process_status = $NS1_BACKUP->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: ns1 returned $process_status\n";
    exit 1;
}

if ($ns1->WaitForFileTimed ($ns1_ior,
                            $ns1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns1_ior>\n";
    $NS1_BACKUP->Kill (); $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

$process_status = $NS2_PRIMARY->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: ns2 returned $process_status\n";
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

if ($ns2->WaitForFileTimed ($ns2_replica_primary_ior,
                            $ns2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$name_dir2/ns_replica_primary.ior>\n";
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

$process_status = $NS2_BACKUP->Spawn ();
if ($process_status != 0) {
    print STDERR "ERROR: ns2 returned $process_status\n";
    $NS2_PRIMARY->Kill (); $NS2_PRIMARY->TimedWait (1);
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

if ($ns2->WaitForFileTimed ($ns2_ior,
                            $ns2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns2_iorfile>\n";
    $NS2_BACKUP->Kill ();  $NS2_BACKUP->TimedWait (1);
    $NS2_PRIMARY->Kill (); $NS2_PRIMARY->TimedWait (1);
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

$process_status = $SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $NS2_BACKUP->Kill ();  $NS2_BACKUP->TimedWait (1);
    $NS2_PRIMARY->Kill (); $NS2_PRIMARY->TimedWait (1);
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $NS2_BACKUP->Kill ();  $NS2_BACKUP->TimedWait (1);
    $NS2_PRIMARY->Kill (); $NS2_PRIMARY->TimedWait (1);
    $NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
    $NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "Starting client\n";
$process_status = $CL->SpawnWaitKill (15);

if ($process_status != 0) {
    print STDERR "ERROR: client returned $process_status\n";
    $status = 1;
}

$process_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());
if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $status = 1;
}

$process_status = $CL->TerminateWaitKill ($client->ProcessStopWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: client returned $process_status\n";
    $status = 1;
}

$NS2_BACKUP->Kill ();  $NS2_BACKUP->TimedWait (1);
$NS2_PRIMARY->Kill (); $NS2_PRIMARY->TimedWait (1);
$NS1_BACKUP->Kill ();  $NS1_BACKUP->TimedWait (1);
$NS1_PRIMARY->Kill (); $NS1_PRIMARY->TimedWait (1);

exit $status;
