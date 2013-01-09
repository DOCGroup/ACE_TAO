eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

#$ENV{ACE_TEST_VERBOSE} = "1";

my $startdir = getcwd();
my $debug_level = '0';
my $redirection_enabled = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-verbose') {
        $redirection_enabled = 0;
    }

}

my $server  = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client  = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";



# Variables for command-line arguments to client and server
# executables.

my $stdout_file        = "test.out";
my $stderr_file        = "test.err";
my $client_stdout_file = $client->LocalFile ($stdout_file);
my $client_stderr_file = $client->LocalFile ($stderr_file);

my $status = 0;
my $POSITIVE_TEST_RESULT = 0;
my $NEGATIVE_TEST_RESULT = 1;

my $NSGROUP = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");
my $NSLIST  = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist");
my $NSADD   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd");
my $NSDEL   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel");

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
    open STDERR, '>', $client_stderr_file;
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
            cat_file($client_stderr_file);
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
            cat_file($client_stderr_file);
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
            cat_file($client_stderr_file);
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
            cat_file($client_stderr_file);
            cat_file($client_stdout_file);
        }
        $status = 1;
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

my $name_dir    = "NameService";
my $group_dir   = "GroupService";
my $nm_iorfile  = "nm.ior";
my $ns_iorfile  = "ns.ior";
my $sv_iorfile  = "obj.ior";
my $ns_ref      = "--ns file://$ns_iorfile";
my $sv2_iorfile = $server2->LocalFile ($sv_iorfile);




################################################################################
# setup END block to cleanup after exit call
################################################################################
END
{
    $server->DeleteFile ($ns_iorfile);
    $server->DeleteFile ($nm_iorfile);
    $client->DeleteFile ($nm_iorfile);
    $client->DeleteFile ($stdout_file);
    $client->DeleteFile ($stderr_file);
    $server2->DeleteFile($sv_iorfile);

    if ( -d $name_dir ) {
        print STDERR "INFO: removing <$name_dir>\n";
        clean_persistence_dir ($server, $name_dir);
        rmdir ($name_dir);
    }

    if ( -d $group_dir ) {
        print STDERR "INFO: removing <$group_dir>\n";
        clean_persistence_dir ($server, $group_dir);
        rmdir ($group_dir);
    }
}

################################################################################
# Validate that repository data written by the name service is available upon
# startup.
################################################################################
sub persistence_test ()
{
    my $previous_status = $status;
    $status = 0;

    my $hostname          = $server->HostName ();
    my $ns_orb_port1      = 10001 + $server->RandomPort ();
    my $ns_endpoint1      = "iiop://$hostname:$ns_orb_port1";
    my $default_init_ref  = "-ORBDefaultInitRef corbaloc:iiop:$hostname:$ns_orb_port1";
    my $client_nm_iorfile = $client->LocalFile ($nm_iorfile);

    print_msg("Persistence Test");
    init_naming_context_directory ($server, $name_dir );
    init_naming_context_directory ($server, $group_dir );

    my $ns_args       = "-ORBListenEndPoints $ns_endpoint1 ".
                        "-g $nm_iorfile ".
                        "-o $ns_iorfile ".
                        "-v $group_dir ".
                        "-u $name_dir ";

    my $tao_ft_naming = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_ft_naming";

    my $client1_args = "--persistence " .
                       "--create " .
                       "-p corbaloc:iiop:$hostname:$ns_orb_port1/NameService " .
                       "-r corbaloc:iiop:$hostname:$ns_orb_port1/NamingManager " .
                       "-b 4 " .
                       "-d 4 ";

    my $client2_args = "--persistence " .
                       "--validate " .
                       "-p corbaloc:iiop:$hostname:$ns_orb_port1/NameService " .
                       "-r corbaloc:iiop:$hostname:$ns_orb_port1/NamingManager " .
                       "-b 4 " .
                       "-d 4 ";

    my $client_prog = "$startdir/client";


    ##1. Run one instance of tao_ft_naming service
    $NS1 = $server->CreateProcess ($tao_ft_naming, $ns_args);
    $CL1 = $client->CreateProcess ($client_prog, $client1_args);
    $CL2 = $client->CreateProcess ($client_prog, $client2_args);


    my $server2_args = "-ORBdebuglevel $debug_level " .
                       "$default_init_ref ".
                       "-o $sv2_iorfile ";

    $SV2 = $server2->CreateProcess ("$startdir/server", $server2_args);

    $server->DeleteFile ($ns_iorfile);
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($ns_iorfile,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$ns_iorfile>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        exit 1;
    }

    ##2. Create new contexts
    ##3. Create a new object group
    print_msg("INFO: starting test server");
    $server_status = $SV2->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        exit 1;
    }
    if ($server2->WaitForFileTimed ($sv_iorfile,
                                   $server2->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$sv_iorfile>\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        exit 1;
    }

    ##4. Verify the new context and object group
    print_msg("INFO: Starting client1");
    $client_status = $CL1->SpawnWaitKill ($client->ProcessStartWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client1 returned $client_status\n";
        $status = 1;
    }

    ##5. Kill the tao_ft_naming server
    print_msg("Kill the tao_ft_naming server");
    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    ##6. Start a new instance of the tao_ft_naming server
    print_msg("Start a new instance of the tao_ft_naming server");
    $server->DeleteFile ($ns_iorfile);
    $NS1->Spawn ();
    if ($server->WaitForFileTimed ($ns_iorfile,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$ns_iorfile>\n";
        $NS1->Kill (); $NS1->TimedWait (1);
        $status = 1;
    }

    ##7. Verify the new name, object group and member are in the tao_ft_naming repository.
    print_msg("Verify the new name, object group and member are in the tao_ft_naming repository");
    print_msg("INFO: Starting client2");
    $client_status = $CL2->SpawnWaitKill ($client->ProcessStartWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client2 returned $client_status\n";
        $status = 1;
    }

    print_msg("INFO: terminating test server");
    $server_status = $SV2->TerminateWaitKill ($server2->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server_status = $NS1->TerminateWaitKill ($server->ProcessStopWaitInterval());
    if ($server_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    if ( $status == 0 ) {
        $status = $previous_status;
    }

    return $status;
}

sub print_msg($)
{
    my $msg = shift;
    my $bar = "===============================================================================";
    print STDERR "\n\n$bar\n$msg\n$bar\n";
}

sub show_result($$)
{
    my $test_result = shift;
    my $test_name = shift;

    if ( 0 == $test_result ) {
        print_msg("$test_name: SUCCESS");
    } else {
        print_msg("$test_name: ERROR");
    }
}

my $result = persistence_test ();

show_result($result, "Persistence Test");

exit $result;
