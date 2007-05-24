##$Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This test currently does not perform
#    ImplRepo_Service -m (multicast)
#    ImplRepo_Service -l (loack database)
#    ImplRepo_Service -c (install/remove Windows service)
#    tao_imr shutdown -l hostname (shutdown servers on a specific host)
#    tao_imr shutdown_repo (shutting down ImplRepo_Service via tao_imr)

#use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;
use File::Spec;
use Getopt::Long;


# Initialize variables
my %TestParams;
my $IMPL_IOR = "-ORBInitRef ImplRepoService=file://implrepo.ior";
my $ENV_CMD;
my $IMR_CMD;
my $ACT_CMD;
my $IOR_CMD;
my $SERVER_CMD;
my $CLIENT_CMD;
my $MGR;
my $IMPL;
my $ACT;
my $SERVER;

# Get command line params
$TestParams{file} = "./drivers/all";
GetOptions("file|f=s" => \$TestParams{file});
parse_file();

# Open the Param file and run the tests within
sub parse_file
{
    my $fh;
    my $file = $TestParams{file};

    my $disabled_block = 0;
    if (open($fh, $file))
    {
        while (<$fh>)
        {
            my $line = $_;
            $line =~ s/\s+$//;

            # If the line is negated, ignore it
            if ($line =~ /^--/)
            {
                $disabled_block = ! ($disabled_block);
            }

            elsif ($disabled_block == 0)
            {
                # If the line is negated, ignore it
                if ($line =~ /^-/)
                {
                }

                # If the line is a comment, print it
                elsif ($line =~ /^#/)
                {
                    print "$line\n";
                }

                # Start defining an execution
                elsif ($line =~ /^begin/)
                {
                    $TestParams{file} = "./drivers/defaults";
                    parse_file();
                }

                # Parse a new file
                elsif ($line =~ /^run (\w+)/)
                {
                    $TestParams{file} = "./drivers/$1";
                    parse_file();
                }

                # Acquire parameter
                elsif ($line =~ /(\w+)=(.*)/)
                {
                    &acquire_param($1, $2);
                }

                # Execute a single line
                elsif ($line =~ /^do (\w+)/)
                {
                    my $methodRef = $1;
                    &$methodRef(); #how do I do this in the strict mode?
                }
            }
        }
    }
}

sub invoke_stats()
{
    my $IORS = ($TestParams{server_poas} * $TestParams{server_poa_objs});
    my $THREAD_REQ = ($TestParams{client_iterations} * $IORS * $TestParams{client_requests});
    my $TOTAL_REQ = ($TestParams{client_threads} * $THREAD_REQ);
    
    my $RAND_MSG = "";
    if ($TestParams{client_rand_req} ne "0")
    {
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
    unlink "manager.ior";
    $MGR = new PerlACE::Process("manager_main", "-r $TestParams{activate_retry}");
    $MGR->Spawn();
    PerlACE::waitforfile_timed("manager.ior", $PerlACE::wait_interval_for_process_creation);

    if ($TestParams{imr_on} eq "1")
    {
        # Start the IMR Service
        unlink "implrepo.ior";
        $IMPL = new PerlACE::Process(
                    "$PerlACE::TAO_ROOT/orbsvcs/ImplRepo_Service/ImplRepo_Service", 
                    $IMR_CMD);
        $IMPL->Spawn();
        PerlACE::waitforfile_timed("implrepo.ior", $PerlACE::wait_interval_for_process_creation);
    }
}

sub kill_imr
{
    if ($TestParams{imr_on} eq "1")
    {
        if ($TestParams{imr_shutdown} ne "0")
        {
            my $tool_params = "$IMPL_IOR shutdown-repo";
            if ($TestParams{imr_shutdown} eq "2" && $TestParams{activate_mode} ne "")
            {
              $tool_params = "$tool_params -a";
            }

            my $TOOL = new PerlACE::Process("$PerlACE::ACE_ROOT/bin/tao_imr", $tool_params);
            $TOOL->IgnoreExeSubDir(1);
            $TOOL->SpawnWaitKill(5);
            
            # test $IMPL
            if ($TestParams{imr_shutdown} eq "2" && $TestParams{activate_mode} ne "")
            {
              $ACT->WaitKill(5);
              print "* Activator Killed.\n";
            }
            $IMPL->WaitKill(5);
        }
        else
        {
            $IMPL->TerminateWaitKill(1);
        }
        print "* IMR Killed.\n";
    }

    my $TEMP_SERV = new PerlACE::Process("server_main", "-q 1 -x 0");
    $TEMP_SERV->SpawnWaitKill(5);
    $MGR->Kill();
}

# write_iors
# creates the IORS for all the objects the client will invoke
# The POAs will be temporarely registered with the IMR.
sub write_iors
{
    generateCommands();
    unlink "imr_test.ior";
    if ($TestParams{activate_mode} eq "per_client")
    {
        # Run a single fake server for every POA
        for (my $i = 1; $i <= $TestParams{server_poas}; $i++)
        {
            my $TEMP_SERV = new PerlACE::Process("server_main", "$IOR_CMD -p 1 -d " . $i);
            $TEMP_SERV->SpawnWaitKill(5);
        }
        print "* IORs Written\n";
    }
    elsif ($TestParams{activate_mode} ne "")
    {
        # Run a single fake server for all POAs
        my $TEMP_SERV = new PerlACE::Process("server_main", "$IOR_CMD -d 1 -p $TestParams{server_poas}");
        $TEMP_SERV->SpawnWaitKill(5);
        PerlACE::waitforfile_timed("imr_test.ior", $PerlACE::wait_interval_for_process_creation);
        print "* IORs Written\n"; 
    }
    else
    {
        # Without an activator, the server will write the IORs when manually launched
    }
}

sub start_activator
{
    if ($TestParams{activate_mode} ne "")
    {
        generateCommands();

        # Start the Activator
        unlink "activator.ior";
        $ACT = new PerlACE::Process(
                "$PerlACE::TAO_ROOT/orbsvcs/ImplRepo_Service/ImR_Activator", 
                $ACT_CMD);
        $ACT->Spawn();
        PerlACE::waitforfile_timed("activator.ior", $PerlACE::wait_interval_for_process_creation);
    }
}

sub kill_activator()
{
    if ($TestParams{activate_mode} ne "")
    {
        if ($TestParams{imr_shutdown} ne "2")
        {
            $ACT->TerminateWaitKill(1);
            print "* Activator Killed.\n";
        }
    }
}

# activator_list
# Lists the known servers
sub activator_list()
{
    my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR list -v");
    $TOOL->IgnoreExeSubDir(1);
    $TOOL->SpawnWaitKill(5);
    
    # TODO: parse output and check values
}

# register_server
# Makes the activator know about the server
# Does nothing for the no-activator mode
sub register_server()
{
    if ($TestParams{activate_mode} eq "per_client")
    {
        # Register a single POA for each server
        generateCommands();
        my $actname = hostname;
        for (my $i = 1; $i <= $TestParams{server_poas}; $i++)
        {
            my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR $TestParams{registration_type} POA_".$i."_1 -c \"server_main $SERVER_CMD -d ".$i." -p 1\" -a $TestParams{activate_mode} -r $TestParams{activate_retry} -l $actname $ENV_CMD");
            $TOOL->IgnoreExeSubDir(1);
            $TOOL->SpawnWaitKill(5);
        }
    }
    elsif ($TestParams{activate_mode} ne "")
    {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        generateCommands();
        my $actname = hostname;
        my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR $TestParams{registration_type} POA_1_1 -c \"server_main $SERVER_CMD -d 1 -p $TestParams{server_poas}\" -a $TestParams{activate_mode} -r $TestParams{activate_retry} -l $actname $ENV_CMD");
        $TOOL->IgnoreExeSubDir(1);
        $TOOL->SpawnWaitKill(5);
    }
    else
    {
        # nothing to register without an activator
    }
}

sub unregister_server()
{
    if ($TestParams{activate_mode} eq "per_client")
    {
        # Register a single POA for each server
        generateCommands();
        my $actname = hostname;
        for (my $i = 1; $i <= $TestParams{server_poas}; $i++)
        {
            my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR remove POA_".$i."_1");
            $TOOL->IgnoreExeSubDir(1);
            $TOOL->SpawnWaitKill(5);
        }
    }
    elsif ($TestParams{activate_mode} ne "")
    {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        generateCommands();
        my $actname = hostname;
        my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR remove POA_1_1");
        $TOOL->IgnoreExeSubDir(1);
        $TOOL->SpawnWaitKill(5);
    }
    else
    {
        # nothing is registered without an activator
    }
}


# start_server
# This method is called before the client is luanched.
# It only launches the server for the two manual modes (no-act and manual-act).
sub start_server()
{
    if ($TestParams{activate_mode} eq "")
    {
        generateCommands();
        $SERVER = new PerlACE::Process("server_main", "$SERVER_CMD -d 1 -o 1 -p $TestParams{server_poas}");
        $SERVER->Spawn();
        PerlACE::waitforfile_timed("imr_test.ior", $PerlACE::wait_interval_for_process_creation);
    }
    elsif ($TestParams{activate_mode} eq "manual")
    {
        activate_server();
    }
    else
    {
        # The server will be launched by the ImR or Client
    }
}

# Activate Server
# This method is an alternative way to start the server.
# It will activate the servers regardless of mode.
sub activate_server
{
    if ($TestParams{activate_mode} eq "per_client")
    {
        # Register a single POA for each server
        generateCommands();
        my $actname = hostname;
        for (my $i = 1; $i <= $TestParams{server_poas}; $i++)
        {
            my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR activate POA_".$i."_1");
            $TOOL->IgnoreExeSubDir(1);
            $TOOL->SpawnWaitKill(5);
        }
    }
    elsif ($TestParams{activate_mode} ne "")
    {
        # Register only the first POA for our server
        # The remaining POAs will auto-register when the server is started
        generateCommands();
        my $actname = hostname;
        my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR activate POA_1_1");
        $TOOL->IgnoreExeSubDir(1);
        $TOOL->SpawnWaitKill(5);
    }
    else # No activator
    {
        start_server();
    }
}

# kill server
# This method is called when the services of the server are no longer required.
# per_client does nothing.
sub kill_server()
{
    if ($TestParams{activate_mode} eq "per_client")
    {
        # clients do this
    }
    elsif ($TestParams{activate_mode} ne "")
    {
        # Kill the single server
        my $TOOL = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr", "$IMPL_IOR shutdown POA_1_1");
        $TOOL->SpawnWaitKill(5);
    }
    else
    {
        $SERVER->Kill();
        print "* Server Killed.\n";
    }
}

# launch_client
# This method spawns the client and blocks until the client is done
sub launch_client
{
    generateCommands();
    if ($TestParams{activate_mode} eq "per_client")
    {
        my $CLIENT = new PerlACE::Process("client_main", "$CLIENT_CMD -x 1");
        $CLIENT->SpawnWaitKill(500);
    }
    else
    {
        my $CLIENT = new PerlACE::Process("client_main", "$CLIENT_CMD");
        $CLIENT->SpawnWaitKill(500);
    }
}

# Private methods

# acquire_param
# Transforms certain parameters into usable forms
sub acquire_param
{
    $TestParams{$_[0]} = $_[1];

    if ($_[0] eq "imr_persistence")
    {
        if ($TestParams{imr_persistence} eq "registry")
        {
            $TestParams{imr_persistence} = "-r";
        }
        elsif ($TestParams{imr_persistence} eq "binary")
        {
            $TestParams{imr_persistence} = "-p imr_persist";
        }
        elsif ($TestParams{imr_persistence} eq "XML")
        {
            $TestParams{imr_persistence} = "-x imr_persist";
        }
    }

    elsif ($_[0] eq "imr_persistence_clear")
    {
        if ($TestParams{imr_persistence_clear} eq "1")
        {
            $TestParams{imr_persistence_clear} = "-e";
        }
        else
        {
            $TestParams{imr_persistence_clear} = "";
        }
    }

    elsif ($_[0] eq "server_ref_style" && $TestParams{server_ref_style} ne "")
    {
        $TestParams{server_ref_style} = "-ORBObjRefStyle $TestParams{server_ref_style}";
    }
    
    elsif ($_[0] eq "client_rand_req" && $TestParams{client_rand_req} eq "1")
    {
        $TestParams{client_rand_req} = "r";
    }

    elsif ($_[0] eq "client_holding" && $TestParams{client_holding} eq "1")
    {
        $TestParams{client_holding} = "h";
    }

    elsif ($_[0] eq "client_noprofile" && $TestParams{client_noprofile} eq "1")
    {
        $TestParams{client_noprofile} = "n";
    }
}

# generateCommands
# Create common cammand line sets
sub generateCommands
{
    $ENV_CMD = "";
    for (my $i = 0; $i < $TestParams{env_varnum}; $i++)
    {
        $ENV_CMD = "$ENV_CMD -e Name_" . $i . "=Value_" . $i . " ";
    }

    if ($TestParams{env_dir} ne "")
    {
        $ENV_CMD = "$ENV_CMD -w " . File::Spec->rel2abs($TestParams{env_dir}) . " ";
    }

    $IMR_CMD    = "-o implrepo.ior "
                . "$TestParams{imr_persistence} "
                . "$TestParams{imr_persistence_clear} "
                . "-ORBEndPoint $TestParams{imr_endpoint} "
                . "-d $TestParams{imr_debug_level} "
                . "-v $TestParams{imr_ping_interval} "
                . "-t $TestParams{imr_startup_timeout} ";

    $ACT_CMD    = "$IMPL_IOR -o activator.ior -d $TestParams{imr_debug_level}";

    $SERVER_CMD = "-ORBUseIMR $TestParams{imr_on} $IMPL_IOR "
                . "$ENV_CMD "
                . "$TestParams{server_ref_style} "
                . "-b " . File::Spec->rel2abs(".") . " "
                . "-t $TestParams{server_ior_table} "
                . "-z $TestParams{env_pause_type} "
                . "-s $TestParams{server_start_pause} "
                . "-c $TestParams{server_obj_act_pause} "
                . "-a $TestParams{server_activate_pause} "
                . "-r $TestParams{server_run_pause} "
                . "-n $TestParams{server_poa_objs} "
                . "-x $TestParams{server_hit_time_min} ";

    $CLIENT_CMD = "-z $TestParams{env_pause_type} "
                . "-s $TestParams{client_start_pause} "
                . "-t $TestParams{client_threads} "
                . "-i $TestParams{client_iterations} "
                . "-r $TestParams{client_rand_req}$TestParams{client_requests} "
                . "-e $TestParams{client_holding}$TestParams{client_noprofile} ";

    $IOR_CMD    = "-ORBUseIMR $TestParams{imr_on} $IMPL_IOR "
                . "$TestParams{server_ref_style} "
                . "-b " . File::Spec->rel2abs(".") . " "
                . "-t $TestParams{server_ior_table} "
                . "-n $TestParams{server_poa_objs} "
                . "-z s -s 100 -o 1 -x 0 " # sleep 100, write iors, and exit
}
