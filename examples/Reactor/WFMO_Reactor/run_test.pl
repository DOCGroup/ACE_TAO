eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

require Process;
use lib '../../../bin';
use PerlACE::Run_Test;

#
# These tests only run on Win32
#
if ($^O ne "MSWin32") 
{
    exit;
}

$test_timeout = 60;

@tests = 
    (
     "Abandoned",
     "APC",
#    "Console_Input",            # This test is interactive
     "Directory_Changes",
     "Exceptions",
     "Handle_Close",
     "Multithreading",
#    "Network_Events",           # This test is interactive
     "Prerun_State_Changes",
     "Registration",
     "Registry_Changes",
     "Removals",
     "Suspended_Removals",
#    "Talker",                   # This test is interactive
     "Timeouts",
     "Window_Messages",
     );

for $test (@tests)
{
    print STDOUT "\n________________________________________\n";
    print STDOUT "\nStarting test \"$test\"";
    print STDOUT "\n________________________________________\n\n";
    
    $test_process = new PerlACE::Process ($test);
    $test_process->Spawn ();
    $test_result = $test_process->WaitKill ($test_timeout);
        
    if ($test_result != 0) 
    {
        print STDERR "\n________________________________________\n";
        print STDERR "\nERROR: \"$test\" returned $test_result";
        print STDERR "\n________________________________________\n";
    }

    print STDOUT "\n________________________________________\n";
    print STDOUT "\n\"$test\" completed";
    print STDOUT "\n________________________________________\n";
}
