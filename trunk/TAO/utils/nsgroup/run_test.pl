eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;
my $debug_level = 0;
my $redirection_enabled = 0;


foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

#$ENV{ACE_TEST_VERBOSE} = "1";

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
my $NM_conf = $server->LocalFile ("windows" . $PerlACE::svcconf_ext);

my $name_mgr_iorbase = "nm.ior";
my $name_srv_iorbase = "ns.ior";
my $stdout_file      = "test.out";
my $stderr_file      = "test.err";

my $server_hostname = $server->HostName ();
my $name_mgr_iorfile = $server->LocalFile ($name_mgr_iorbase);
my $name_server_iorfile = $server->LocalFile ($name_srv_iorbase);

my $naming_mgr_client_iorfile = $client->LocalFile ($name_mgr_iorbase);
my $name_srv_client_iorfile = $client->LocalFile ($name_srv_iorbase);
my $client_stdout_file = $client->LocalFile ($stdout_file);
my $client_stderr_file = $client->LocalFile ($stderr_file);

$server->DeleteFile($name_mgr_iorbase);
$server->DeleteFile($name_srv_iorbase);
$client->DeleteFile($name_mgr_iorbase);
$client->DeleteFile($name_srv_iorbase);
$client->DeleteFile($stdout_file);
$client->DeleteFile($stderr_file);

my $DEBUG_LEVEL   = "-ORBDebugLevel $debug_level";
my $hostname = $server->HostName ();
my $ns_orb_port1  = 10001 + $server->RandomPort ();
my $ns_endpoint1  = "iiop://$hostname:$ns_orb_port1";

my $DEF_REF  = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1";
#my $NM_REF   = "-ORBInitRef NameService=file://$name_srv_client_iorfile";
#my $RM_REF   = "-ORBInitRef NamingManager=file://$naming_mgr_client_iorfile";
my $NS_REF   = "--ns file://$name_srv_client_iorfile";
my $LOAD_ARG = "$DEF_REF $DEBUG_LEVEL";

my $tao_ft_naming = "$ENV{TAO_ROOT}/orbsvcs/FT_Naming_Service/tao_ft_naming";
my $name_dir      = "NameService";
my $group_dir     = "Groups";
my $ns_args       = "$DEBUG_LEVEL " .
                    "-ORBListenEndPoints $ns_endpoint1 " .
                    "-h $name_mgr_iorbase " .
                    "-o $name_srv_iorbase " .
                    "-v $group_dir " .
                    "-u $name_dir " .
                    ($^O eq 'MSWin32' ? "-ORBSvcConf $NM_conf" : '');

my $NM      = $server->CreateProcess ($tao_ft_naming, $ns_args);
my $NSGROUP = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");
my $NSLIST  = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist");
my $NSADD   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd");
my $NSDEL   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel");

my $POSITIVE_TEST_RESULT = 0;
my $NEGATIVE_TEST_RESULT = 1;

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

sub redirect_output()
{
    open(OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open(OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stderr_file;
    open STDOUT, '>', $client_stdout_file;
}

sub restore_output()
{
    open(STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!";
    open(STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!";
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

    $NSGROUP->Arguments ($arglist);

    if ($redirection_enabled) {
        redirect_output();
    }

    my $client_status = $NSGROUP->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }


    if ($client_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: client returned $client_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stderr_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
    }
}

sub run_nsadd($)
{
    print STDERR "\n\n======== Running tao_nsadd ================\n";
    my $args = shift;
    $NSADD->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nsadd --ns file://ns.ior --name iso --ctx
    my $client_status = $NSADD->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($client_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nsadd returned $client_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stderr_file);
        }
        $status = 1;
    }
}

sub run_nsdel($)
{
    print STDERR "\n\n======== Running tao_nsdel ================\n";
    my $args = shift;
    $NSDEL->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nsdel --ns file://ns.ior --name iso --destroy
    my $client_status = $NSDEL->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($client_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nsdel returned $client_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stderr_file);
        }
        $status = 1;
    }
}

sub run_nslist($)
{
    print STDERR "\n\n======== Running tao_nslist ================\n";
    my $args = shift;
    $NSLIST->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nslist --ns file://ns.ior
    my $client_status = $NSLIST->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($client_status != $0) {
        my $time = localtime;
        print STDERR "ERROR: nslist returned $client_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stderr_file);
        }
        $status = 1;
    }
}

sub run_clients ()
{
    run_client (
        "group_list",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieee -policy round",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieed -policy rand",
        $POSITIVE_TEST_RESULT);

    run_client (
        "group_create -group ieee -policy round",
        $NEGATIVE_TEST_RESULT);

    run_client (
        "group_list",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_add -group ieee -location $server_hostname -ior file://$naming_mgr_client_iorfile",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_nsadd("$DEF_REF"." --name iso --ctx");

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

    # Change the policy back to a supported one before
    # doing the nslist to avoid the error message.
    run_client (
        "group_modify -group ieee -policy round",
        $POSITIVE_TEST_RESULT);

    run_nslist("$NS_REF");

    run_client (
        "member_add -group ieee -location $server_hostname -ior file://$naming_mgr_client_iorfile",
        $NEGATIVE_TEST_RESULT);

    run_client (
        "member_list -group ieee",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_show -group ieee -location $server_hostname",
        $POSITIVE_TEST_RESULT);

    run_client (
        "member_remove -group ieee -location $server_hostname",
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

    run_nsdel("$DEF_REF"." --name iso --destroy");

    # Verify we can handle a non-existent ior to add to the list
    run_nsadd("$DEF_REF --name does_not_exist --ior file://thisfiledoesnotexist");
    run_nslist("$NS_REF");
    run_nsdel("$DEF_REF"." --name does_not_exist");
    run_nslist("$NS_REF");


    run_client (
        "-help",
        $POSITIVE_TEST_RESULT);
}

print STDERR "\n\n======== Running tao_nsgroup Test================\n";
print STDERR "\n";

print STDERR "This test will check the methods of the tao_nsgroup\n";
print STDERR "\n";

init_persistence_directory ($server, $name_dir );
init_persistence_directory ($server, $group_dir );

################################################################################
# setup END block to cleanup after exit call
################################################################################
END
{
    $server->DeleteFile($name_mgr_iorbase);
    $server->DeleteFile($name_srv_iorbase);
    $client->DeleteFile($name_mgr_iorbase);
    $client->DeleteFile($name_srv_iorbase);
    $client->DeleteFile($stdout_file);
    $client->DeleteFile($stderr_file);

    if ( -d $name_dir ) {
        clean_persistence_dir ($server, $name_dir);
        rmdir ($name_dir);
    }

    if ( -d $group_dir ) {
        clean_persistence_dir ($server, $group_dir);
        rmdir ($group_dir);
    }
}

################################################################################
## Start tao_ft_naming Service
################################################################################

$server_status = $NM->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($name_mgr_iorbase,
        $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$name_mgr_iorbase>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

print STDERR "Waiting for $name_mgr_iorbase\n";
if ($server->GetFile ($name_mgr_iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$name_mgr_iorbase>\n";
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
$server_status = $NM->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}


exit $status;
