eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

$startdir = getcwd();

my $test     = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nsgroup  = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $nslist   = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $nsadd    = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $nsdel    = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

# Variables for command-line arguments to client and server
# executables.
$hostname = $test->HostName ();

$ns_orb_port1 = 10001 + $test->RandomPort ();
$ns_orb_port2 = 10002 + $test->RandomPort ();

$ns_endpoint1 = "iiop://$hostname:$ns_orb_port1";
$ns_endpoint2 = "iiop://$hostname:$ns_orb_port2";

my $iorfile1     = "ns1.ior";
my $iorfile2     = "ns2.ior";

# References to both naming services
my $default_init_ref    = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1,iiop:$hostname:$ns_orb_port2";
my $name_service_ref    = "-ORBInitRef NameService=corbaloc:iiop:$hostname:$ns_orb_port1,iiop:$hostname:$ns_orb_port2/NameService";
my $naming_manager_ref  = "-ORBInitRef NamingManager=corbaloc:iiop:$hostname:$ns_orb_port1,iiop:$hostname:$ns_orb_port2/ObjectGroupManager";
my $nsgroup_refs        = "$name_service_ref $naming_manager_ref";

# References to primary naming service only
my $primary_default_init_ref   = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1";
my $primary_name_service_ref   = "-ORBInitRef NameService=corbaloc:iiop:$hostname:$ns_orb_port1/NameService";
my $primary_naming_manager_ref = "-ORBInitRef NamingManager=corbaloc:iiop:$hostname:$ns_orb_port1/ObjectGroupManager";
my $primary_nsgroup_refs       = "$primary_name_service_ref $primary_naming_manager_ref";

# References to secondary naming service only
my $backup_default_init_ref    = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port2";
my $backup_name_service_ref    = "-ORBInitRef NameService=corbaloc:iiop:$hostname:$ns_orb_port2/NameService";
my $backup_naming_manager_ref  = "-ORBInitRef NamingManager=corbaloc:iiop:$hostname:$ns_orb_port2/ObjectGroupManager";
my $secondary_nsgroup_refs     = "$secondary_name_service_ref $secondary_naming_manager_ref";

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

my $test_iorfile1 = $test->LocalFile ($iorfile1);
my $test_iorfile2 = $test->LocalFile ($iorfile2);

$status = 0;
$POSITIVE_TEST_RESULT = 0;
$NEGATIVE_TEST_RESULT = 1;

$NSGROUP = $nsgroup->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");
$NSLIST  = $nslist->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist");
$NSADD   = $nsadd->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd");
$NSDEL   = $nsdel->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel");

sub run_nsgroup ($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    my $arglist = "$args";

    if ($expected_test_result != $POSITIVE_TEST_RESULT ) {
        print STDERR "\n\n======== Running Negative Test ================\n";
    } else {
        print STDERR "\n\n======== Running Positive Test ================\n";
    }
    print STDERR "$args\n";

    $NSGROUP->Arguments ($arglist);

    my $nsgroup_status = $NSGROUP->SpawnWaitKill ($nsgroup->ProcessStartWaitInterval());

    if ($nsgroup_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsgroup returned $nsgroup_status at $time\n";
        $status = 1;
    } else {
        print STDERR "SUCCESS\n";
    }
}

sub run_nslist($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    if ($expected_test_result != $POSITIVE_TEST_RESULT ) {
        print STDERR "\n\n======== Running Negative Test ================\n";
    } else {
        print STDERR "\n\n======== Running Positive Test ================\n";
    }
    print STDERR "$args\n";

    $NSLIST->Arguments ($args);

    #tao_nslist --ns file://ns.ior
    my $nslist_status = $NSLIST->SpawnWaitKill ($nslist->ProcessStartWaitInterval());

    if ($nslist_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nslist returned $nslist_status at $time\n";
        $status = 1;
    } else {
        print STDERR "SUCCESS\n";
    }
}

sub run_nsadd($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    if ($expected_test_result != $POSITIVE_TEST_RESULT ) {
        print STDERR "\n\n======== Running Negative Test ================\n";
    } else {
        print STDERR "\n\n======== Running Positive Test ================\n";
    }
    print STDERR "$args\n";

    $NSADD->Arguments ($args);

    #tao_nsadd --ns file://ns.ior --name iso --ctx
    my $nsadd_status = $NSADD->SpawnWaitKill ($nsadd->ProcessStartWaitInterval());

    if ($nsadd_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsadd returned $nsadd_status at $time\n";
        $status = 1;
    } else {
        print STDERR "SUCCESS\n";
    }
}

sub run_nsdel($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    if ($expected_test_result != $POSITIVE_TEST_RESULT ) {
        print STDERR "\n\n======== Running Negative Test ================\n";
    } else {
        print STDERR "\n\n======== Running Positive Test ================\n";
    }
    print STDERR "$args\n";

    $NSDEL->Arguments ($args);

    #tao_nsdel --ns file://ns.ior --name iso --destroy
    my $nsdel_status = $NSDEL->SpawnWaitKill ($nsdel->ProcessStartWaitInterval());

    if ($nsdel_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsdel returned $nsdel_status at $time\n";
        $status = 1;
    } else {
        print STDERR "SUCCESS\n";
    }
}

print "INFO: Running the test in ", getcwd(), "\n";

# Make sure that the directory to use to hold the naming contexts exists
# and is cleaned out
if ( ! -d "NameService" ) {
    mkdir (NameService, 0777);
} else {
    chdir "NameService";
    opendir(THISDIR, ".");
    @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
    closedir(THISDIR);
    foreach $tmp (@allfiles){
        $test->DeleteFile ($tmp);
    }
    chdir "..";
}

# Run two Naming Servers
my $ns1_args = "-ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint1 -o $test_iorfile1 -m 0 -r NameService";
my $ns2_args = "-ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint2 -o $test_iorfile2 -m 0 -r NameService";

$NS1 = $test->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns1_args");
$NS2 = $test->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns2_args");

$test->DeleteFile ("$iorfile1");
$NS1->Spawn ();
if ($test->WaitForFileTimed ($iorfile1,
                               $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_iorfile1>\n";
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

$test->DeleteFile ("$iorfile2");
$NS2->Spawn ();
if ($test->WaitForFileTimed ($iorfile2,
                               $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_iorfile2>\n";
    $NS2->Kill (); $NS2->TimedWait (1);
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

print STDERR "\n\n======== Tests with primary and secondary servers running ================\n";
run_nsadd ("$default_init_ref --name iso --ctx", $POSITIVE_TEST_RESULT);
run_nsgroup ("$nsgroup_refs group_create -group ieee -policy round -type_id IDL:FT/NamingManager:1.0", $POSITIVE_TEST_RESULT);
run_nsgroup ("$nsgroup_refs group_bind -group ieee -name iso/ieee", $POSITIVE_TEST_RESULT);
run_nsgroup ("$nsgroup_refs group_list", $POSITIVE_TEST_RESULT);
run_nslist ("$default_init_ref", $POSITIVE_TEST_RESULT);

print STDERR "\n\n======== Kill primary server ================\n";
$server_status = $NS1->TerminateWaitKill ($test->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    $status = 1;
}

print STDERR "\n\n======== Verify primary server is not running ================\n";
run_nsgroup ("$primary_nsgroup_refs group_list", $NEGATIVE_TEST_RESULT);
run_nslist ("$primary_default_init_ref", $NEGATIVE_TEST_RESULT);

print STDERR "\n\n======== Test failover with only secondary server running ================\n";
run_nsgroup ("$nsgroup_refs group_list", $POSITIVE_TEST_RESULT);
run_nslist ("$default_init_ref", $POSITIVE_TEST_RESULT);

print STDERR "\n\n======== Verify restarted primary server is available================\n";
$test->DeleteFile ("$iorfile1");
$NS1->Spawn ();
if ($test->WaitForFileTimed ($iorfile1,
                               $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_iorfile1>\n";
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

run_nsgroup ("$primary_nsgroup_refs group_list", $POSITIVE_TEST_RESULT);
run_nslist ("$primary_default_init_ref", $POSITIVE_TEST_RESULT);

print STDERR "\n\n======== Test complete ================\n";

$server_status = $NS2->TerminateWaitKill ($test->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $status = 1;
}

$server_status = $NS1->TerminateWaitKill ($test->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    $status = 1;
}

$test->DeleteFile($iorfile1);
$test->DeleteFile($iorfile2);

if ($status != 0) {
    print STDERR "ERROR: Test Failed\n";
} else {
    print STDERR "SUCCESS: Test Passed\n";
}

exit $status;
