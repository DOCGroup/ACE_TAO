eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

my $debug_level = '0';
my $redirection_enabled = 1;


foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
        $redirection_enabled = 0;
    }
}

my $server  = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client  = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


# Variables for command-line arguments to client and server
# executables.
my $hostname = $server->HostName ();

my $ns_orb_port1 = 10001 + $server->RandomPort ();
my $ns_orb_port2 = 10002 + $server->RandomPort ();

my $ns_endpoint1 = "iiop://$hostname:$ns_orb_port1";
my $ns_endpoint2 = "iiop://$hostname:$ns_orb_port2";

my $iorfile1     = "ns1.ior";
my $iorfile2     = "ns2.ior";
my $stdout_file = "test.out";
my $stderr_file = "test.err";

my $client_stdout_file = $client->LocalFile ($stdout_file);
my $client_stdout_file = $client->LocalFile ($stderr_file);

# References to both naming services
my $default_init_ref = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1,iiop:$hostname:$ns_orb_port2";

# References to primary naming service only
my $primary_default_init_ref = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1";

# References to backup naming service only
my $backup_default_init_ref = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port2";

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

my $server_iorfile1 = $server->LocalFile ($iorfile1);
my $server_iorfile2 = $server->LocalFile ($iorfile2);

my $status = 0;
my $POSITIVE_TEST_RESULT = 0;
my $NEGATIVE_TEST_RESULT = 1;

my $NSGROUP = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");
my $NSLIST  = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist");
my $NSADD   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd");
my $NSDEL   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel");


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
    open (OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open (OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $client_stdout_file;
    open STDOUT, '>', $client_stdout_file;
}

sub restore_output()
{
    open (STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!";
    open (STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!";
}

sub run_nsgroup ($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    my $arglist = "$args";

    $NSGROUP->Arguments ($arglist);

    if ($redirection_enabled) {
        redirect_output();
    }

    my $nsgroup_status = $NSGROUP->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($nsgroup_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsgroup returned $nsgroup_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stdout_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
    }
}

sub run_nslist($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    $NSLIST->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nslist --ns file://ns.ior
    my $nslist_status = $NSLIST->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($nslist_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nslist returned $nslist_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stdout_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
    }
}

sub run_nsadd($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    $NSADD->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nsadd --ns file://ns.ior --name iso --ctx
    my $nsadd_status = $NSADD->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($nsadd_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsadd returned $nsadd_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stdout_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
    }
}

sub run_nsdel($$)
{
    my $args = shift;
    my $expected_test_result = shift;

    $NSDEL->Arguments ($args);

    if ($redirection_enabled) {
        redirect_output();
    }

    #tao_nsdel --ns file://ns.ior --name iso --destroy
    my $nsdel_status = $NSDEL->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($redirection_enabled) {
        restore_output();
    }

    if ($nsdel_status != $expected_test_result) {
        my $time = localtime;
        print STDERR "ERROR: nsdel returned $nsdel_status at $time\n";
        if ($redirection_enabled) {
            cat_file($client_stdout_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
    }
}

# Make sure that the directory to use to hold the naming contexts exists
# and is cleaned out
sub init_naming_context_directory($$)
{
    my $target = shift;
    my $directory_name = shift;

    if ( ! -d $directory_name ) {
        mkdir (NameService, 0777);
    } else {
        chdir $directory_name;
        opendir(THISDIR, ".");
        @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
        closedir(THISDIR);
        foreach $tmp (@allfiles){
            $target->DeleteFile ($tmp);
        }
        chdir "..";
    }
}

################################################################################
# Validate that a client can seamlessly connect to the alternate server of a
# server naming server pair after the other server has been terminated.
################################################################################
sub FailoverTest()
{
    my $previous_status = $status;
    $status = 0;
    my $NAME_CONTEXT_DIRECTORY = "NameService";


    print STDERR "\n\n==== Failover Test =======================================\n";
    init_naming_context_directory ($server, $NAME_CONTEXT_DIRECTORY );

    # // Start the primary. It will write the replication ior to a file in the persistence dir.
    # tao_ft_naming --primary -ORBEndpoint <primary_hostname:port> -r <nameService_persistence_dir>

    # // Start the backup. Reads the primary ior from the persistence dir. Writes the multi-profile ior to naming_ior_filename.
    # tao_ft_naming --backup -ORBEndpoint <primary_hostname:port> -r <nameService_persistence_dir> -o <naming_ior_filename>

    # Run two Naming Servers
    my $ns1_args = "--primary -ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint1 -o $server_iorfile1 -m 0 -r $NAME_CONTEXT_DIRECTORY";
    my $ns2_args = "--backup -ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint2 -o $server_iorfile2 -m 0 -r $NAME_CONTEXT_DIRECTORY";

    $NS1 = $server->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns1_args");
    $NS2 = $server->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns2_args");

    $server->DeleteFile ("$iorfile1");
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($iorfile1,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    $server->DeleteFile ("$iorfile2");
    $NS2->Spawn ();
    if ($server->WaitForFileTimed ($iorfile2,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
        $NS2->Kill (); $NS2->TimedWait (1);
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    run_nsadd ("$default_init_ref --name iso --ctx", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$default_init_ref group_create -group ieee -policy round -type_id IDL:FT/NamingManager:1.0", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$default_init_ref group_bind -group ieee -name iso/ieee", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$default_init_ref group_list", $POSITIVE_TEST_RESULT);
    run_nslist ("$default_init_ref", $POSITIVE_TEST_RESULT);

    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    #Verify primary server is not available
    run_nsgroup ("$primary_default_init_ref group_list", $NEGATIVE_TEST_RESULT);
    run_nslist ("$primary_default_init_ref", $NEGATIVE_TEST_RESULT);

    #Test failover with only secondary server running
    run_nsgroup ("$default_init_ref group_list", $POSITIVE_TEST_RESULT);
    run_nslist ("$default_init_ref", $POSITIVE_TEST_RESULT);

    #restart primary server
    $server->DeleteFile ("$iorfile1");
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($iorfile1,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    #Verify restarted primary server is available
    run_nsgroup ("$primary_default_init_ref group_list", $POSITIVE_TEST_RESULT);
    run_nslist ("$primary_default_init_ref", $POSITIVE_TEST_RESULT);

    $server_status = $NS2->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 2 returned $server_status\n";
        $status = 1;
    }

    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorfile1);
    $server->DeleteFile($iorfile2);

    print STDERR "\n\n==== Failover Test Result=================================\n";
    if ( $status == 0 ) {
        $status = $previous_status;
        print STDERR "SUCCESS\n";
    } else {
        print STDERR "ERROR\n";
    }
    print STDERR "==========================================================\n";

}

################################################################################
# Validate that repository data written by the name service is available upon
# startup.
################################################################################
sub PersistanceTest ()
{

    my $previous_status = $status;
    $status = 0;
    my $NAME_CONTEXT_DIRECTORY = "NameService";

    my $nm_iorfile  = "nm.ior";
    my $server_nm_iorfile = $server->LocalFile ($nm_iorfile);
    my $client_nm_iorfile = $client->LocalFile ($nm_iorfile);


    print STDERR "\n\n==== Persistance Test ====================================\n";
    init_naming_context_directory ($server, $NAME_CONTEXT_DIRECTORY );

    # Run two Naming Servers
    my $ns_args = "--primary -ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint1 -o $server_iorfile1 -g $server_nm_iorfile -m 0 -r $NAME_CONTEXT_DIRECTORY";

    ##1. Run one instance of tao_ft_naming service
    $NS1 = $server->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns_args");

    $server->DeleteFile ("$iorfile1");
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($iorfile1,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    ##2. Create a new context with tao_nsadd
    run_nsadd ("$primary_default_init_ref --name iso --ctx", $POSITIVE_TEST_RESULT);

    ##3. Create a new object group and add a member with tao_nsgroup and bind the object group to a name
    run_nsgroup ("$primary_default_init_ref group_create -group ieee -policy round -type_id IDL:FT/NamingManager:1.0", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$primary_default_init_ref member_add -group ieee -location $hostname -ior file://$client_nm_iorfile", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$primary_default_init_ref group_bind -group ieee -name iso/ieee", $POSITIVE_TEST_RESULT);

    ##4. Verify the new name, object group and member are in the tao_ft_naming repository.
    run_nsgroup ("$primary_default_init_ref group_list", $POSITIVE_TEST_RESULT);
    run_nslist ("$primary_default_init_ref", $POSITIVE_TEST_RESULT);

    ##5. Kill the tao_ft_naming server
    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    ##6. Start a new instance of the tao_ft_naming server
    $server->DeleteFile ("$iorfile1");
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($iorfile1,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    ##7. Verify the new name, object group and member are in the tao_ft_naming repository.
    run_nsgroup ("$primary_default_init_ref member_remove -group ieee -location $hostname", $POSITIVE_TEST_RESULT);
    #run_nsgroup ("$primary_default_init_ref group_unbind -name iso/ieee", $POSITIVE_TEST_RESULT);
    #run_nsgroup ("$primary_default_init_ref group_remove -group ieee", $POSITIVE_TEST_RESULT);
    #run_nsdel ("$primary_default_init_ref --name iso --destroy", $POSITIVE_TEST_RESULT);
    ##run_nsgroup ("$primary_default_init_ref group_list", $POSITIVE_TEST_RESULT);
    ##run_nslist ("$primary_default_init_ref", $POSITIVE_TEST_RESULT);

    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorfile1);

    print STDERR "\n\n==== Persistance Test Result==============================\n";
    if ( $status == 0 ) {
        $status = $previous_status;
        print STDERR "SUCCESS\n";
    } else {
        print STDERR "ERROR\n";
    }
    print STDERR "==========================================================\n";


}

################################################################################
# Validate that a client can seamlessly invoke naming operations on either
# server instance.
################################################################################
sub RedundantEquivalancyTest()
{
    my $previous_status = $status;
    $status = 0;
    my $NAME_CONTEXT_DIRECTORY = "NameService";

    print STDERR "\n\n==== Redundant Equivalancy Test ==========================\n";

    init_naming_context_directory ($server, $NAME_CONTEXT_DIRECTORY);

    # Run two Naming Servers
    my $ns1_args = "--primary -ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint1 -o $server_iorfile1 -m 0 -r $NAME_CONTEXT_DIRECTORY";
    my $ns2_args = "--backup -ORBDebugLevel $debug_level -ORBListenEndPoints $ns_endpoint2 -o $server_iorfile2 -m 0 -r $NAME_CONTEXT_DIRECTORY";

    $NS1 = $server->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns1_args");
    $NS2 = $server->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming", "$ns2_args");

    $server->DeleteFile ("$iorfile1");
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($iorfile1,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    $server->DeleteFile ("$iorfile2");
    $NS2->Spawn ();
    if ($server->WaitForFileTimed ($iorfile2,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile2>\n";
        $NS2->Kill (); $NS2->TimedWait (1);
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    #print STDERR "\n\n==== Tests with primary and secondary servers running ====\n";
    run_nsadd ("$primary_default_init_ref --name iso --ctx", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$backup_default_init_ref group_create -group ieee -policy round -type_id IDL:FT/NamingManager:1.0", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$primary_default_init_ref group_bind -group ieee -name iso/ieee", $POSITIVE_TEST_RESULT);
    run_nsgroup ("$backup_default_init_ref group_list", $POSITIVE_TEST_RESULT);
    run_nslist ("$primary_default_init_ref", $POSITIVE_TEST_RESULT);

    #print STDERR "\n\n==== Test complete =======================================\n";

    $server_status = $NS2->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 2 returned $server_status\n";
        $status = 1;
    }

    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorfile1);
    $server->DeleteFile($iorfile2);

    print STDERR "\n\n==== Redundant Equivalancy Test Result====================\n";
    if ( $status == 0 ) {
        $status = $previous_status;
        print STDERR "SUCCESS\n";
    } else {
        print STDERR "ERROR\n";
    }
    print STDERR "==========================================================\n";
}

FailoverTest ();
PersistanceTest ();
RedundantEquivalancyTest ();

if ($status != 0) {
    print STDERR "ERROR: One or more tests Failed\n";
} else {
    print STDERR "SUCCESS: All Tests Passed\n";
}

exit $status;
