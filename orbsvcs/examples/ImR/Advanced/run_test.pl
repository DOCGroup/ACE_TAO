eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;
use Getopt::Long;

my $status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $imrtarget = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $acttarget = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $titarget = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $manager = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

my $ACTHOSTNAME = $acttarget->HostName ();

my $iriorbase = "zimplrepo.ior";
my $irpersistbase = "imr_persist";
my $imr_iriorfile = $imrtarget->LocalFile ($iriorbase);
my $act_iriorfile = $acttarget->LocalFile ($iriorbase);
my $ti_iriorfile = $titarget->LocalFile ($iriorbase);
my $sv_iriorfile = $server->LocalFile ($iriorbase);
my $imr_irpersistfile = $imrtarget->LocalFile ($irpersistbase);
$imrtarget->DeleteFile ($iriorbase);
$acttarget->DeleteFile ($iriorbase);
$titarget->DeleteFile ($iriorbase);
$server->DeleteFile ($iriorbase);
$imrtarget->DeleteFile ($irpersistbase);

my $actiorbase = "zactivator.ior";
my $act_actiorfile = $acttarget->LocalFile ($actiorbase);
$acttarget->DeleteFile ($actiorbase);

my $mgriorbase = "zmanager.ior";
my $mgr_mgriorfile = $manager->LocalFile ($mgriorbase);
my $sv_mgriorfile = $server->LocalFile ($mgriorbase);
$manager->DeleteFile ($mgriorbase);
$server->DeleteFile ($mgriorbase);

my $sviorbase = "zimr_test.ior";
my $sv_sviorfile = $server->LocalFile ($sviorbase);
my $cl_sviorfile = $client->LocalFile ($sviorbase);
$server->DeleteFile ($sviorbase);
$client->DeleteFile ($sviorbase);

# Initialize variables
my %TestParams;
my $IMR_CMD;
my $ACT_CMD;
my $SV_IOR_CMD;
my $SERVER_CMD;
my $CLIENT_CMD;

my $MGR = $manager->CreateProcess ("manager_main");
my $IMPL = $imrtarget->CreateProcess (
    "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator");
my $ACT = $acttarget->CreateProcess (
    "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator");
my $SV = $server->CreateProcess ("server_main");
my $TI = $titarget->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_imr");
$TI->IgnoreExeSubDir (1);
my $CL = $client->CreateProcess ("client_main");

# Get command line params
$TestParams{file} = "./drivers/all";
GetOptions ("file|f=s" => \$TestParams{file});
$TestParams{file} = $server->LocalFile ($TestParams{file});
parse_file ();

$imrtarget->DeleteFile ($iriorbase);
$acttarget->DeleteFile ($iriorbase);
$titarget->DeleteFile ($iriorbase);
$server->DeleteFile ($iriorbase);
$imrtarget->DeleteFile ($irpersistbase);

$acttarget->DeleteFile ($actiorbase);

$manager->DeleteFile ($mgriorbase);
$server->DeleteFile ($mgriorbase);

$server->DeleteFile ($sviorbase);
$client->DeleteFile ($sviorbase);

# The script is done. There are only subroutines below exit.
exit $status;


# Open the Param file and run the tests within
sub parse_file
{
    my $fh;
    my $file = $TestParams{file};

    my $disabled_block = 0;
    if (open($fh, $file)) {
        while (<$fh>) {
            my $line = $_;
            $line =~ s/\s+$//;

            # If the line is negated, ignore it
            if ($line =~ /^--/) {
                $disabled_block = ! ($disabled_block);
            }
            elsif ($disabled_block == 0) {
                # If the line is negated, ignore it
                if ($line =~ /^-/) {
                }

                # If the line is a comment, print it
                elsif ($line =~ /^#/) {
                    print "$line\n";
                }

                # Start defining an execution
                elsif ($line =~ /^begin/) {
                    $TestParams{file} =
                        $server->LocalFile ("./drivers/defaults");
                    parse_file();
                }

                # Parse a new file
                elsif ($line =~ /^run (\w+)/) {
                    $TestParams{file} =
                        $server->LocalFile ("./drivers/$1");
                    parse_file();
                }

                # Acquire parameter
                elsif ($line =~ /(\w+)=(.*)/) {
                    &acquire_param ($1, $2);
                }

                # Execute a single line
                elsif ($line =~ /^do (\w+)/) {
                    my $methodRef = $1;
                    &$methodRef(); #how do I do this in the strict mode?
                }
            }
        }
    }
}

sub invoke_stats
{
    my $IORS = ($TestParams{server_poas} * $TestParams{server_poa_objs});
    my $THREAD_REQ = ($TestParams{client_iterations} * $IORS * $TestParams{client_requests});
    my $TOTAL_REQ = ($TestParams{client_threads} * $THREAD_REQ);

    my $RAND_MSG = "";
    if ($TestParams{client_rand_req} ne "0") {
        $RAND_MSG = "(max random)";
    }
    print "* Invocation Stats\n";
    print "* POAs:               $TestParams{server_poas}\n";
    print "* Obj/POA:            $TestParams{server_poa_objs}\n";
    print "* IORs:               $IORS\n";
    print "* Threads:            $TestParams{client_threads}\n";
    print "* Iterations/Thread:  $TestParams{client_iterations}\n";
    print "* Requests/Thread:    $THREAD_REQ $RAND_MSG\n";
    print "* Total Requests:     $TOTAL_REQ $RAND_MSG\n";
}

sub start_imr
{
    generateCommands();

    # Start the manager
    $manager->DeleteFile ($mgriorbase);
    $server->DeleteFile ($mgriorbase);

    $MGR->Arguments ("-o $mgr_mgriorfile "
                     . "-r $TestParams{activate_retry}");

    $mgr_status = $MGR->Spawn ();
    if ($mgr_status != 0) {
        print STDERR "ERROR: manager returned $mgr_status\n";
        clean_bail ();
    }
    if ($manager->WaitForFileTimed (
            $mgriorbase,
            $manager->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: cannot find file <$mgr_mgriorfile>\n";
        clean_bail ();
    }

    if ($TestParams{imr_on} ne "1") {
        return;
    }

    # Start the IMR Service
    $imrtarget->DeleteFile ($iriorbase);
    $acttarget->DeleteFile ($iriorbase);
    $titarget->DeleteFile ($iriorbase);
    $server->DeleteFile ($iriorbase);

    $IMPL->Arguments ($IMR_CMD);

    $imr_status = $IMPL->Spawn ();
    if ($imr_status != 0) {
        print STDERR "ERROR: ImplRepo_Service returned $imr_status\n";
        clean_bail ();
    }
    if ($imrtarget->WaitForFileTimed (
            $iriorbase,
            $imrtarget->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: cannot find file <$imr_iriorfile>\n";
        clean_bail ();
    }
    if ($imrtarget->GetFile ($iriorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$imr_iriorfile>\n";
        clean_bail ();
    }
    if ($acttarget->PutFile ($iriorbase) == -1) {
        print STDERR "ERROR: cannot set file <$act_iriorfile>\n";
        clean_bail ();
    }
    if ($titarget->PutFile ($iriorbase) == -1) {
        print STDERR "ERROR: cannot set file <$ti_iriorfile>\n";
        clean_bail ();
    }
    if ($server->PutFile ($iriorbase) == -1) {
        print STDERR "ERROR: cannot set file <$sv_iriorfile>\n";
        clean_bail ();
    }
}

sub kill_imr
{
    if ($TestParams{imr_on} eq "1") {
        if ($TestParams{imr_shutdown} ne "0") {
            my $tool_params =
                "-ORBInitRef ImplRepoService=file://$ti_iriorfile shutdown-repo";
            if ($TestParams{imr_shutdown} eq "2" &&
                $TestParams{activate_mode} ne "") {
              $tool_params = "$tool_params -a";
            }

            $TI->Arguments ($tool_params);
            $ti_status = $TI->SpawnWaitKill (
                $titarget->ProcessStartWaitInterval ());
            if ($ti_status != 0) {
                print STDERR "ERROR: tao_imr returned $ti_status\n";
                clean_bail ();
            }

            # test $IMPL
            if ($TestParams{imr_shutdown} eq "2" &&
                $TestParams{activate_mode} ne "") {
                $act_status = $ACT->WaitKill (
                    $acttarget->ProcessStopWaitInterval ());
                if ($act_status != 0) {
                    print STDERR "ERROR: ImR_Activator returned $act_status\n";
                    clean_bail ();
                }
                print "* Activator Killed.\n";
            }
            $imr_status = $IMPL->WaitKill (
                $imrtarget->ProcessStopWaitInterval ());
            if ($imr_status != 0) {
                print STDERR "ERROR: ImplRepo_Service returned $imr_status\n";
                clean_bail ();
            }
        }
        else {
            $imr_status = $IMPL->TerminateWaitKill (
                $imrtarget->ProcessStopWaitInterval ());
            if ($imr_status != 0) {
                print STDERR "ERROR: ImplRepo_Service returned $imr_status\n";
                clean_bail ();
            }
        }
        print "* IMR Killed.\n";
    }

    $SV->Arguments ("-q 1 -x 0 -m file://$sv_mgriorfile");
    $sv_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval ());
    if ($sv_status != 0) {
        print STDERR "ERROR: server returned $sv_status\n";
        clean_bail ();
    }

    $mgr_status = $MGR->TerminateWaitKill ($manager->ProcessStopWaitInterval ());
    if ($mgr_status != 0) {
        print STDERR "ERROR: manager returned $mgr_status\n";
        clean_bail ();
    }
}

# write_iors
# creates the IORS for all the objects the client will invoke
# The POAs will be temporarely registered with the IMR.
sub write_iors
{
    generateCommands();

    $server->DeleteFile ($sviorbase);
    $client->DeleteFile ($sviorbase);

    if ($TestParams{activate_mode} eq "per_client") {
        # Run a single fake server for every POA
        for (my $i = 1; $i <= $TestParams{server_poas}; $i++) {
            $SV->Arguments ("$SV_IOR_CMD -i $sv_sviorfile -p 1 -d $i");
            $sv_status =
                $SV->SpawnWaitKill ($server->ProcessStartWaitInterval ());
            if ($sv_status != 0) {
                print STDERR "ERROR: server returned $sv_status\n";
                clean_bail ();
            }
            if ($server->WaitForFileTimed (
                    $sviorbase,
                    $server->ProcessStartWaitInterval ()) == -1) {
                print STDERR "ERROR: cannot find file <$sv_sviorfile>\n";
                clean_bail ();
            }
            if ($server->GetFile ($sviorbase) == -1) {
                print STDERR "ERROR: cannot retrieve file <$sv_sviorfile>\n";
                clean_bail ();
            }
            if ($client->PutFile ($sviorbase) == -1) {
                print STDERR "ERROR: cannot set file <$cl_sviorfile>\n";
                clean_bail ();
            }
        }
        print "* IORs Written\n";
    }
    elsif ($TestParams{activate_mode} ne "") {
        # Run a single fake server for all POAs
        $SV->Arguments ("$SV_IOR_CMD -i $sv_sviorfile -d 1 "
                        . "-p $TestParams{server_poas}");
        $sv_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval ());
        if ($sv_status != 0) {
            print STDERR "ERROR: server returned $sv_status\n";
            clean_bail ();
        }
        if ($server->WaitForFileTimed (
                $sviorbase,
                $server->ProcessStartWaitInterval ()) == -1) {
            print STDERR "ERROR: cannot find file <$sv_sviorfile>\n";
            clean_bail ();
        }
        if ($server->GetFile ($sviorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$sv_sviorfile>\n";
            clean_bail ();
        }
        if ($client->PutFile ($sviorbase) == -1) {
            print STDERR "ERROR: cannot set file <$cl_sviorfile>\n";
            clean_bail ();
        }
        print "* IORs Written\n";
    }
    else {
        # Without an activator, the server will write
        # the IORs when manually launched
    }
}

sub start_activator
{
    if ($TestParams{activate_mode} ne "") {
        generateCommands();

        # Start the Activator
        $acttarget->DeleteFile ($actiorbase);

        $ACT->Arguments ($ACT_CMD);
        $act_status = $ACT->Spawn ();
        if ($act_status != 0) {
            print STDERR "ERROR: ImR_Activator returned $act_status\n";
            clean_bail ();
        }
        if ($acttarget->WaitForFileTimed (
                $actiorbase,
                $acttarget->ProcessStartWaitInterval ()) == -1) {
            print STDERR "ERROR: cannot find file <$act_actiorfile>\n";
            clean_bail ();
        }
    }
}

sub kill_activator
{
    if ($TestParams{activate_mode} ne "") {
        if ($TestParams{imr_shutdown} ne "2") {
            $act_status =
                $ACT->TerminateWaitKill ($acttarget->ProcessStopWaitInterval ());
            if ($act_status != 0) {
                print STDERR "ERROR: ImR_Activator returned $act_status\n";
                clean_bail ();
            }
            print "* Activator Killed.\n";
        }
    }
}

# activator_list
# Lists the known servers
sub activator_list
{
    $TI->Arguments (
        "-ORBInitRef ImplRepoService=file://$ti_iriorfile list -v");
    $ti_status = $TI->SpawnWaitKill (
        $titarget->ProcessStartWaitInterval ());
    if ($ti_status != 0) {
        print STDERR "ERROR: tao_imr returned $ti_status\n";
        clean_bail ();
    }

    # TODO: parse output and check values
}

# register_server
# Makes the activator know about the server
# Does nothing for the no-activator mode
sub register_server
{
    if ($TestParams{activate_mode} eq "per_client") {
        # Register a single POA for each server
        my $ti_env_cmd = generateCommands();
        my $sv_executable = $server->LocalFile ($SV->Executable ());

        for (my $i = 1; $i <= $TestParams{server_poas}; $i++) {
            $TI->Arguments (
                "-ORBInitRef ImplRepoService=file://$ti_iriorfile "
                . "$TestParams{registration_type} POA_${i}_1 "
                . "-c \"$sv_executable $SERVER_CMD -d $i -p 1\" "
                . "-a $TestParams{activate_mode} "
                . "-r $TestParams{activate_retry} -l $ACTHOSTNAME $ti_env_cmd");
            $ti_status = $TI->SpawnWaitKill (
                $titarget->ProcessStartWaitInterval ());
            if ($ti_status != 0) {
                print STDERR "ERROR: tao_imr returned $ti_status\n";
                clean_bail ();
            }
        }
    }
    elsif ($TestParams{activate_mode} ne "") {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        my $ti_env_cmd = generateCommands();
        my $sv_executable = $server->LocalFile ($SV->Executable ());

        $TI->Arguments (
            "-ORBInitRef ImplRepoService=file://$ti_iriorfile "
            . "$TestParams{registration_type} POA_1_1 "
            . "-c \"$sv_executable $SERVER_CMD -d 1 -p $TestParams{server_poas}\" "
            . "-a $TestParams{activate_mode} -r $TestParams{activate_retry} "
            . "-l $ACTHOSTNAME $ti_env_cmd");
        $ti_status = $TI->SpawnWaitKill (
            $titarget->ProcessStartWaitInterval ());
        if ($ti_status != 0) {
            print STDERR "ERROR: tao_imr returned $ti_status\n";
            clean_bail ();
        }
    }
    else {
        # nothing to register without an activator
    }
}

sub unregister_server
{
    if ($TestParams{activate_mode} eq "per_client") {
        # Register a single POA for each server
        generateCommands();

        for (my $i = 1; $i <= $TestParams{server_poas}; $i++) {
            $TI->Arguments (
                "-ORBInitRef ImplRepoService=file://$ti_iriorfile "
                . "remove POA_${i}_1");
            $ti_status = $TI->SpawnWaitKill (
                $titarget->ProcessStartWaitInterval ());
            if ($ti_status != 0) {
                print STDERR "ERROR: tao_imr returned $ti_status\n";
                clean_bail ();
            }
        }
    }
    elsif ($TestParams{activate_mode} ne "") {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        generateCommands();

        $TI->Arguments (
            "-ORBInitRef ImplRepoService=file://$ti_iriorfile remove POA_1_1");
        $ti_status = $TI->SpawnWaitKill (
            $titarget->ProcessStartWaitInterval ());
        if ($ti_status != 0) {
            print STDERR "ERROR: tao_imr returned $ti_status\n";
            clean_bail ();
        }
    }
    else {
        # nothing is registered without an activator
    }
}

# start_server
# This method is called before the client is luanched.
# It only launches the server for the two manual modes (no-act and manual-act).
sub start_server
{
    if ($TestParams{activate_mode} eq "") {
        generateCommands();

        $SV->Arguments ("$SERVER_CMD -d 1 -o 1 -p $TestParams{server_poas}");
        $sv_status = $SV->Spawn ();
        if ($sv_status != 0) {
            print STDERR "ERROR: server returned $sv_status\n";
            clean_bail ();
        }
        if ($server->WaitForFileTimed (
                $sviorbase,
                $server->ProcessStartWaitInterval ()) == -1) {
            print STDERR "ERROR: cannot find file <$sv_sviorfile>\n";
            clean_bail ();
        }
        if ($server->GetFile ($sviorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$sv_sviorfile>\n";
            clean_bail ();
        }
        if ($client->PutFile ($sviorbase) == -1) {
            print STDERR "ERROR: cannot set file <$cl_sviorfile>\n";
            clean_bail ();
        }
    }
    elsif ($TestParams{activate_mode} eq "manual") {
        activate_server();
    }
    else {
        # The server will be launched by the ImR or Client
    }
}

# Activate Server
# This method is an alternative way to start the server.
# It will activate the servers regardless of mode.
sub activate_server
{
    if ($TestParams{activate_mode} eq "per_client") {
        # Register a single POA for each server
        generateCommands();

        for (my $i = 1; $i <= $TestParams{server_poas}; $i++) {
            $TI->Arguments (
                "-ORBInitRef ImplRepoService=file://$ti_iriorfile "
                . "activate POA_${i}_1");
            $ti_status = $TI->SpawnWaitKill (
                $titarget->ProcessStartWaitInterval ());
            if ($ti_status != 0) {
                print STDERR "ERROR: tao_imr returned $ti_status\n";
                clean_bail ();
            }
        }
    }
    elsif ($TestParams{activate_mode} ne "") {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        generateCommands();

        $TI->Arguments (
            "-ORBInitRef ImplRepoService=file://$ti_iriorfile "
            . "activate POA_1_1");
        $ti_status = $TI->SpawnWaitKill (
            $titarget->ProcessStartWaitInterval ());
        if ($ti_status != 0) {
            print STDERR "ERROR: tao_imr returned $ti_status\n";
            clean_bail ();
        }
    }
    else {
        # No activator
        start_server();
    }
}

# kill server
# This method is called when the services of the server are no longer required.
# per_client does nothing.
sub kill_server
{
    if ($TestParams{activate_mode} eq "per_client") {
        # clients do this
    }
    elsif ($TestParams{activate_mode} ne "") {
        # Kill the single server
        $TI->Arguments (
            "-ORBInitRef ImplRepoService=file://$ti_iriorfile shutdown POA_1_1");
        $ti_status = $TI->SpawnWaitKill (
            $titarget->ProcessStartWaitInterval ());
        if ($ti_status != 0) {
            print STDERR "ERROR: tao_imr returned $ti_status\n";
            clean_bail ();
        }
    }
    else {
        $sv_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval ());
        if ($sv_status != 0) {
            print STDERR "ERROR: server returned $sv_status\n";
            clean_bail ();
        }
        print "* Server Killed.\n";
    }
}

# launch_client
# This method spawns the client and blocks until the client is done
sub launch_client
{
    generateCommands();

    if ($TestParams{activate_mode} eq "per_client") {
        $CL->Arguments ("$CLIENT_CMD -x 1");
        $cl_status = $CL->SpawnWaitKill (
            $client->ProcessStartWaitInterval () + 485);
        if ($cl_status != 0) {
            print STDERR "ERROR: client returned $cl_status\n";
            clean_bail ();
        }
    }
    else {
        $CL->Arguments ("$CLIENT_CMD");
        $cl_status = $CL->SpawnWaitKill (
            $client->ProcessStartWaitInterval () + 485);
        if ($cl_status != 0) {
            print STDERR "ERROR: client returned $cl_status\n";
            clean_bail ();
        }
    }
}

# Private methods

# acquire_param
# Transforms certain parameters into usable forms
sub acquire_param
{
    $TestParams{$_[0]} = $_[1];

    if ($_[0] eq "imr_persistence") {
        if ($TestParams{imr_persistence} eq "registry") {
            $TestParams{imr_persistence} = "-r";
        }
        elsif ($TestParams{imr_persistence} eq "binary") {
            $TestParams{imr_persistence} = "-p $imr_irpersistfile";
        }
        elsif ($TestParams{imr_persistence} eq "XML") {
            $TestParams{imr_persistence} = "-x $imr_irpersistfile";
        }
    }

    elsif ($_[0] eq "imr_persistence_clear") {
        if ($TestParams{imr_persistence_clear} eq "1") {
            $TestParams{imr_persistence_clear} = "-e";
        }
        else {
            $TestParams{imr_persistence_clear} = "";
        }
    }

    elsif ($_[0] eq "server_ref_style" && $TestParams{server_ref_style} ne "") {
        $TestParams{server_ref_style} =
            "-ORBObjRefStyle $TestParams{server_ref_style}";
    }

    elsif ($_[0] eq "client_rand_req" && $TestParams{client_rand_req} eq "1") {
        $TestParams{client_rand_req} = "r";
    }

    elsif ($_[0] eq "client_holding" && $TestParams{client_holding} eq "1") {
        $TestParams{client_holding} = "h";
    }

    elsif ($_[0] eq "client_noprofile" && $TestParams{client_noprofile} eq "1") {
        $TestParams{client_noprofile} = "n";
    }

    elsif ($_[0] eq "env_dir") {
        $TestParams{env_dir} = $server->LocalFile ($TestParams{env_dir});
    }
}

# generateCommands
# Create common cammand line sets
sub generateCommands
{
    my $sv_base_dir = $server->LocalFile (".");

    my $env_cmd = "";
    for (my $i = 0; $i < $TestParams{env_varnum}; $i++) {
        $env_cmd = "$env_cmd -e Name_$i=Value_$i ";
    }

    if ($TestParams{env_dir} ne "") {
        $env_cmd = "$env_cmd -w $TestParams{env_dir} ";
    }

    $IMR_CMD    = "-o $imr_iriorfile "
        . "$TestParams{imr_persistence} "
        . "$TestParams{imr_persistence_clear} "
        . "-ORBListenEndpoints $TestParams{imr_endpoint} "
        . "-d $TestParams{imr_debug_level} "
        . "-v $TestParams{imr_ping_interval} "
        . "-t $TestParams{imr_startup_timeout} ";

    $ACT_CMD    = "-ORBInitRef ImplRepoService=file://$act_iriorfile "
        . "-o $act_actiorfile -d $TestParams{imr_debug_level} -n $ACTHOSTNAME ";

    $SERVER_CMD = "-ORBUseIMR $TestParams{imr_on} "
        . "-ORBInitRef ImplRepoService=file://$sv_iriorfile "
        . "$env_cmd "
        . "$TestParams{server_ref_style} "
        . "-b $sv_base_dir "
        . "-t $TestParams{server_ior_table} "
        . "-z $TestParams{env_pause_type} "
        . "-s $TestParams{server_start_pause} "
        . "-c $TestParams{server_obj_act_pause} "
        . "-a $TestParams{server_activate_pause} "
        . "-r $TestParams{server_run_pause} "
        . "-n $TestParams{server_poa_objs} "
        . "-x $TestParams{server_hit_time_min} "
        . "-i $sv_sviorfile "
        . "-m file://$sv_mgriorfile ";

    $CLIENT_CMD = "-z $TestParams{env_pause_type} "
        . "-s $TestParams{client_start_pause} "
        . "-t $TestParams{client_threads} "
        . "-i $TestParams{client_iterations} "
        . "-r $TestParams{client_rand_req}$TestParams{client_requests} "
        . "-e $TestParams{client_holding}$TestParams{client_noprofile} "
        . "-k $cl_sviorfile ";

    $SV_IOR_CMD = "-ORBUseIMR $TestParams{imr_on} "
        . "-ORBInitRef ImplRepoService=file://$sv_iriorfile "
        . "$TestParams{server_ref_style} "
        . "-b $sv_base_dir "
        . "-t $TestParams{server_ior_table} "
        . "-n $TestParams{server_poa_objs} "
        . "-z s -s 100 -o 1 -x 0 "; # sleep 100, write iors, and exit

    return $env_cmd;
}

sub clean_bail
{
    # For the moment we don't clean and bail. The test
    # has to be fixed first and then we can uncomment
    # lines below.
    $status = 1;
#     $imrtarget->DeleteFile ($iriorbase);
#     $acttarget->DeleteFile ($iriorbase);
#     $titarget->DeleteFile ($iriorbase);
#     $server->DeleteFile ($iriorbase);
#     $imrtarget->DeleteFile ($irpersistbase);

#     $acttarget->DeleteFile ($actiorbase);

#     $manager->DeleteFile ($mgriorbase);
#     $server->DeleteFile ($mgriorbase);

#     $server->DeleteFile ($sviorbase);
#     $client->DeleteFile ($sviorbase);

#     $MGR->Kill (); $MGR->TimedWait (1);
#     $IMPL->Kill (); $IMPL->TimedWait (1);
#     $ACT->Kill (); $ACT->TimedWait (1);
#     $SV->Kill (); $SV->TimedWait (1);

#     exit 1;
}
