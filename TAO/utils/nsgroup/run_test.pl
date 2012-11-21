eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

#$ENV{ACE_TEST_VERBOSE} = "1";

my $name_manager = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client       = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $nslist       = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $nsadd        = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $nsdel        = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$NM_conf = $name_manager->LocalFile ("windows" . $PerlACE::svcconf_ext);

my $name_mgr_iorbase = "nm.ior";
my $name_srv_iorbase = "ns.ior";
my $stdout_file = "test.out";
my $stderr_file = "test.err";

my $name_manager_iorfile = $name_manager->LocalFile ($name_mgr_iorbase);
my $name_server_iorfile = $name_manager->LocalFile ($name_srv_iorbase);

my $naming_mgr_client_iorfile = $client->LocalFile ($name_mgr_iorbase);
my $name_srv_client_iorfile = $client->LocalFile ($name_srv_iorbase);
my $client_stdout_file = $client->LocalFile ($stdout_file);
my $client_stderr_file = $client->LocalFile ($stderr_file);

$name_manager->DeleteFile($name_mgr_iorbase);
$name_manager->DeleteFile($name_srv_iorbase);
$client->DeleteFile($name_mgr_iorbase);
$client->DeleteFile($name_srv_iorbase);
$client->DeleteFile($stdout_file);
$client->DeleteFile($stderr_file);

$NM = $name_manager->CreateProcess ("../../orbsvcs/Naming_Service/tao_ft_naming",
        "-f persist.dat -g $name_manager_iorfile -o $name_server_iorfile " .
        "-ORBDebugLevel $debug_level " .
        "-ORBDottedDecimalAddresses 1" .
        ($^O eq 'MSWin32' ? " -ORBSvcConf $NM_conf" : ''));

$NM_REF       = "-ORBInitRef NameService=file://$name_srv_client_iorfile";
$RM_REF       = "-ORBInitRef NamingManager=file://$naming_mgr_client_iorfile";
$NS_REF       = "--ns file://$name_srv_client_iorfile";
$DEBUG_LEVEL  = "-ORBDebugLevel $debug_level";
$LOAD_ARG     = "$NM_REF $RM_REF $DEBUG_LEVEL";

$CL   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");
$NSLIST = $nslist->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist");
$NSADD  = $nsadd->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd");
$NSDEL  = $nsdel->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel");

$POSITIVE_TEST_RESULT = 0;
$NEGATIVE_TEST_RESULT = 1;

sub cat_file($)
{
    my $file_name = shift;
    if (-s $file_name ) # size of file is greater than zero
    {
        open TESTFILE, $file_name or die "Couldn't open file: $!";
        my @teststring = <TESTFILE>; # read in all of the file
        print STDERR "\n@teststring\n";
        close TESTFILE;
    }
}

sub run_client ($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    my $arglist = "$LOAD_ARG $args";

    if ($expected_test_result != $POSITIVE_TEST_RESULT ) {
        print STDERR "\n\n======== Running Negative Test ================\n";
    } else {
        print STDERR "\n\n======== Running Positive Test ================\n";
    }
    print STDERR "$args\n";

    $CL->Arguments ($arglist);

    open(OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stderr_file;
    open STDOUT, '>', $client_stdout_file;
    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
    open(STDERR, ">&OLDERR")    or die "Can't dup OLDERR: $!";
    open(STDOUT, ">&OLDOUT")    or die "Can't dup OLDOUT: $!";


    if ($client_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: client returned $client_status at $time\n";
        cat_file($client_stderr_file);
        cat_file($client_stdout_file);
        $status = 1;
    }
}

sub run_nsadd($)
{
    print STDERR "\n\n======== Running tao_nsadd ================\n";
    my $args = shift;
    $NSADD->Arguments ($args);

    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stderr_file;

    #tao_nsadd --ns file://ns.ior --name iso --ctx
    my $nsadd_status = $NSADD->SpawnWaitKill ($nsadd->ProcessStartWaitInterval());
    open(STDERR, ">&OLDERR")    or die "Can't dup OLDERR: $!";

    if ($nsadd_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nsadd returned $nsadd_status at $time\n";
        cat_file($client_stderr_file);
        $status = 1;
    }
}

sub run_nsdel($)
{
    print STDERR "\n\n======== Running tao_nsdel ================\n";
    my $args = shift;
    $NSDEL->Arguments ($args);

    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stderr_file;

    #tao_nsdel --ns file://ns.ior --name iso --destroy
    my $nsdel_status = $NSDEL->SpawnWaitKill ($nsdel->ProcessStartWaitInterval());
    open(STDERR, ">&OLDERR")    or die "Can't dup OLDERR: $!";

    if ($nsdel_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nsdel returned $nsdel_status at $time\n";
        cat_file($client_stderr_file);
        $status = 1;
    }
}

sub run_nslist($)
{
    print STDERR "\n\n======== Running tao_nslist ================\n";
    my $args = shift;
    $NSLIST->Arguments ($args);

    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stderr_file;

    #tao_nslist --ns file://ns.ior
    my $nslist_status = $NSLIST->SpawnWaitKill ($nslist->ProcessStartWaitInterval());

    open(STDERR, ">&OLDERR")    or die "Can't dup OLDERR: $!";

    if ($nslist_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nslist returned $nslist_status at $time\n";
        cat_file($client_stderr_file);
        $status = 1;
    }
}

sub run_clients ()
{
    run_client (
        "group_list",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieee -policy round -type_id IDL:FT/NamingManager:1.0",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieed -policy rand -type_id IDL:/FT/NamingManager:1.0",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieee -policy round -type_id IDL:/FT/NamingManager:1.0",
        $NEGATIVE_TEST_RESULT);

    run_client (
        "group_list",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_add -group ieee -location 127.0.0.1 -ior file://$naming_mgr_client_iorfile",
        $POSITIVE_TEST_RESULT);

    run_nsadd("$NS_REF"." --name iso --ctx");

    run_nslist("$NS_REF");

    run_client (
        "group_unbind -name iso/ieee",
        $NEGATIVE_TEST_RESULT);

    run_nslist("$NS_REF");

    run_client (
        "group_bind -group ieee -name iso/ieee",
        $POSITIVE_TEST_RESULT);

    run_nslist("$NS_REF");

    run_client (
        "group_modify -group ieee -policy rand",
        $POSITIVE_TEST_RESULT);

    run_nslist("$NS_REF");

    run_client (
        "member_add -group ieee -location 127.0.0.1 -ior file://$naming_mgr_client_iorfile",
        $NEGATIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_show -group ieee -location 127.0.0.1",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_remove -group ieee -location 127.0.0.1",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_remove -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_unbind -name iso/ieee",
        $POSITIVE_TEST_RESULT);

    run_nslist("$NS_REF");

    run_client (
        "group_list",
        $POSITIVE_TEST_RESULT);

    run_nsdel("$NS_REF"." --name iso --destroy");

    run_client (
        "-help",
        $POSITIVE_TEST_RESULT);
}

print STDERR "\n\n======== Running tao_nsgroup Test================\n";
print STDERR "\n";

print STDERR "This test will check the methods of the tao_nsgroup\n";
print STDERR "\n";

################################################################################
## Start tao_ft_naming Service
################################################################################
$name_manager_status = $NM->Spawn ();

if ($name_manager_status != 0) {
    print STDERR "ERROR: server returned $name_manager_status\n";
    exit 1;
}
if ($name_manager->WaitForFileTimed ($name_mgr_iorbase,
        $name_manager->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$name_manager_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($name_manager->GetFile ($name_mgr_iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$name_manager_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($name_mgr_iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$naming_mgr_client_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

run_clients();


print STDERR "\n\n====================================================\n";
print STDERR "\n";
$name_manager_status = $NM->TerminateWaitKill ($name_manager->ProcessStopWaitInterval());

if ($name_manager_status != 0) {
    print STDERR "ERROR: server returned $name_manager_status\n";
    $status = 1;
}

$name_manager->DeleteFile($name_mgr_iorbase);
$name_manager->DeleteFile($name_srv_iorbase);
$client->DeleteFile($name_mgr_iorbase);
$client->DeleteFile($name_srv_iorbase);
$client->DeleteFile($stdout_file);
$client->DeleteFile($stderr_file);

exit $status;
