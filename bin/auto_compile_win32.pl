
# $Id$
#   Win32 autobuild helper.

# use lib "$ENV{ACE_ROOT}/bin";

## TO-DO
##   * Added command line options to selective compile library, et.al.
##   * Added cvs update.  Also controllable from command line.
##   * Added building an running version_tests/.  It uses a different
##     naming scheme.
##   * Selectively build some specific configs from command line.

## Available command line switches
##   * -k : Ignore error.  Compile all projects in a collection regardless
##          of errors.
##   * -a : Build Alpha collections.
##   * -D : Build DLLs only.
##   * -L : Build LIBs only.
##   * -d : Debug the script.  No compilation will occur.
##   * -v : Verbose mode.
##   * -r : Rebuild all.  (Default is Build.)

@Win32_Lists= ("Win32 Debug",
               "Win32 Release",
               "Win32 Unicode Debug",
               "Win32 Unicode Release");

@Alpha_Lists= ("Win32 Alpha Debug",
               "Win32 Alpha Release",
               "Win32 Alpha Unicode Debug",
               "Win32 Alpha Unicode Release");

%Win32_DLL_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
TAO/orbsvcs/Scheduling_Service/Scheduling_Service.dsw, Scheduling_Service:
TAO/orbsvcs/Event_Service/Event_Service.dsw, Event_Service:
TAO/orbsvcs/Dump_Schedule/Dump_Schedule.dsw, Dump_Schedule:
TAO/orbsvcs/Concurrency_Service/Concurrency_Service.dsw, Concurrency_Service:
TAO/orbsvcs/LifeCycle_Service/LifeCycle_Service.dsw, LifeCycle_Service:
TAO/orbsvcs/Trading_Service/Trading_Service.dsw, Trading_Service:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Server:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Clerk:
TAO/orbsvcs/ImplRepo_Service/ImplRepo_Service.dsw, ImplRepo_Service:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Server:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Client:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Server:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Client:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
TAO/orbsvcs/Scheduling_Service/Scheduling_Service.dsw, Scheduling_Service:
TAO/orbsvcs/Event_Service/Event_Service.dsw, Event_Service:
TAO/orbsvcs/Dump_Schedule/Dump_Schedule.dsw, Dump_Schedule:
TAO/orbsvcs/Concurrency_Service/Concurrency_Service.dsw, Concurrency_Service:
TAO/orbsvcs/LifeCycle_Service/LifeCycle_Service.dsw, LifeCycle_Service:
TAO/orbsvcs/Trading_Service/Trading_Service.dsw, Trading_Service:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Server:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Clerk:
TAO/orbsvcs/ImplRepo_Service/ImplRepo_Service.dsw, ImplRepo_Service:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Server:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Client:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Server:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Client:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE dynamic library:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE dynamic library:
EOD
                  );

%Alpha_DLL_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
tests/tests.dsw, ALL:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE dynamic library:
apps/gperf/src/gperf.dsw, ALL:
TAO/tao/TAO.dsw, TAO:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, orbsvcs:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE dynamic library:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE dynamic library:
EOD
                  );

%Win32_Lib_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE static library:
TAO/tao/TAO.dsw, TAO Static:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services Static:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE static library:
EOD
                  );

%Alpha_Lib_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE static library:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE static library:
EOD
                  );

@Version_Tests_List = (
                       "Barrier_Test.dsp",
                       "Basic_Types_Test.dsp",
                       "Buffer_Stream_Test.dsp",
                       "CDR_Test.dsp",
                       "Conn_Test.dsp",
                       "DLList_Test.dsp",
                       "Dynamic_Priority_Test.dsp",
                       "Enum_Interfaces_Test.dsp",
                       "Env_Value_Test.dsp",
                       "Future_Test.dsp",
                       "Handle_Set_Test.dsp",
                       "Hash_Map_Manager_Test.dsp",
                       "High_Res_Timer_Test.dsp",
                       "IOStream_Test.dsp",
                       "Map_Manager_Test.dsp",
                       "Mem_Map_Test.dsp",
                       "Message_Block_Test.dsp",
                       "Message_Queue_Notifications_Test.dsp",
                       "Message_Queue_Test.dsp",
                       "MM_Shared_Memory_Test.dsp",
                       "MT_SOCK_Test.dsp",
                       "Naming_Test.dsp",
                       "Notify_Performance_Test.dsp",
                       "OrdMultiSet_Test.dsp",
                       "Pipe_Test.dsp",
                       "Priority_Buffer_Test.dsp",
                       "Priority_Reactor_Test.dsp",
                       "Priority_Task_Test.dsp",
                       "Process_Mutex_Test.dsp",
                       "Process_Strategy_Test.dsp",
                       "RB_Tree_Test.dsp",
                       "Reactors_Test.dsp",
                       "Reactor_Exception_Test.dsp",
                       "Reactor_Notify_Test.dsp",
                       "Reactor_Performance_Test.dsp",
                       "Reactor_Timer_Test.dsp",
                       "Reader_Writer_Test.dsp",
                       "Recursive_Mutex_Test.dsp",
                       "Semaphore_Test.dsp",
                       "Service_Config_Test.dsp",
                       "Sigset_Op_Test.dsp",
                       "Simple_Message_Block_Test.dsp",
                       "SOCK_Connector_Test.dsp",
                       "SOCK_Test.dsp",
                       "SPIPE_Test.dsp",
                       "SString_Test.dsp",
                       "SV_Shared_Memory_Test.dsp",
                       "Task_Test.dsp",
                       "Thread_Manager_Test.dsp",
                       "Thread_Mutex_Test.dsp",
                       "Thread_Pool_Reactor_Test.dsp",
                       "Thread_Pool_Test.dsp",
                       "Timeprobe_Test.dsp",
                       "Timer_Queue_Test.dsp",
                       "Time_Service_Test.dsp",
                       "Time_Value_Test.dsp",
                       "Tokens_Test.dsp",
                       "TSS_Test.dsp",
                       "Upgradable_RW_Test.dsp",
                       "UPIPE_SAP_Test.dsp" );

sub Build_Config
{
    my $Config = shift;
    my $Collection = shift;
    my $Bname = "";
    my $Project_File = "";
    my $Project_Name = "";
    my $Status = -1;
    my $Cntr = 0;
    my $Command_Line = "";

    @Collection = split /:\s*/, $Collection;
    for ($Cntr = 0; $Cntr < scalar (@Collection); $Cntr++)
    {
        $Project_Dir = $Collection[$Cntr];
        $Project_Dir =~ s/(.*)\/[^\/]*/$1/;
        $Bname = $Collection[$Cntr];
        $Bname =~ s/.*\/([^\/]*)/$1/;
        ($Project_File, $Project_Name) = split /,\s*/, $Bname;
        chdir ("$ENV{ACE_ROOT}/$Project_Dir");
        $Command_Line =  "msdev.com $Project_File /MAKE \"$Project_Name - $Config\" /USEENV $Build_Cmd /Y3";
        if ( $Debug == 0 )
        {
            $Status =
                system "$Command_Line";
        }
        else
        {
            $Status = 0;
            print "chdir (\"$ENV{ACE_ROOT}/$Project_Dir\");\n";
            print "$Status = $Command_Line\n";
        }

        if ($Ignore_error == 0)
        {
            return if $Status != 0;
        }
    }
}

sub Build_Version_Test
{
    my $Config = shift;
    my $Cntr = 0;
    my $Command_Line = "";

    chdir ("$ENV{ACE_ROOT}/tests/version_tests");
    for ($Cntr = 0; $Cntr < scalar (@Version_Tests_List); $Cntr++)
    {
        $Project_Name = $Version_Tests_List[$Cntr];
        $Project_Name =~ s/(.*)\.dsp/$1/;
        $Command_Line =  "msdev.com $Version_Tests_List[$Cntr] /MAKE \"$Project_Name - $Config\" /USEENV $Build_Cmd /Y3";
        if ( $Debug == 0 )
        {
            $Status =
                system "$Command_Line";
        }
        else
        {
            $Status = 0;
            print "$Status = $Command_Line\n";
        }

        if ($Ignore_error == 0)
        {
            return if $Status != 0;
        }
    }
}

sub Build_Collection
{
    print "Build_Collection\n" if ( $Verbose );
    my $Cntr = 0;
    for (; $Cntr < scalar(@Lists); $Cntr ++)
    {
        $Config = $Lists[$Cntr];
        print "Building $Config of $Target{$Config}\n" if ( $Debug );
        Build_Config ($Config, $Target{$Config});
        Build_Version_Test ($Config);
    }
}


$Verbose = 0;
$Debug = 0;
$Ignore_error = 0;              # By default, bail out if an error occurs.
$Build_DLL = 1;
$Build_LIB = 1;
$Build_Cmd = "/BUILD";
@Lists = @Win32_Lists;
%DLL_Collections = %Win32_DLL_Collections;
%Lib_Collections = %Win32_Lib_Collections;

## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ( $ARGV[0] eq '-k' )     # Ignore error.  Compile the whole thing
    {
        print "Ignore errors\n" if ( $Verbose );
        $Ignore_error = 1;      # in the same configuration.
    }
    elsif ( $ARGV[0] eq '-a' )  # Use Alpha
    {
        print "Build Alpha\n" if ( $Verbose );
        @Lists = @Alpha_Lists;
        %DLL_Collections = %Alpha_DLL_Collections;
        %Lib_Collections = %Alpha_Lib_Collections;
    }
    elsif ( $ARGV[0] eq '-d')   # Script debugging mode
    {
        print "Debug mode on\nVerbose mode on\n";
        $Debug = 1;
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-D' )  # Build DLL only
    {
        print "Build DLL only\n" if ( $Verbose );
        $Build_LIB = 0;
    }
    elsif ( $ARGV[0] eq '-L' )  # Build LIB only
    {
        print "Build LIB only\n" if ( $Verbose );
        $Build_DLL = 0;
    }
    elsif ( $ARGV[0] eq '-v' )   # Verbose mode
    {
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-r' )  # Rebuild all
    {
        print "Rebuild all\n" if ( $Verbose );
        $Build_Cmd = "/REBUILD";
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

if ( $Verbose )
{
    for ($II = 0; $II < scalar (@Lists); $II++)
    {
        printf "$Lists[$II]\n";
    }
}

if ( $Build_DLL )
{
    print "Building DLL\n" if ( $Verbose );
    %Target = %DLL_Collections;
    Build_Collection;
}

if ( $Build_LIB )
{
    print "Building LIB\n" if ( $Verbose );
    %Target = %Lib_Collections;
    Build_Collection;
}

print "End\n";
