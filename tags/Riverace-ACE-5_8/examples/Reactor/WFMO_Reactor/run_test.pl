eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

#
# These tests only run on Win32
#
if ($^O ne "MSWin32")
{
    exit;
}


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

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";;
$test_timeout = 60 + $target->ProcessStartWaitInterval();

for $test (@tests)
{
    print STDOUT "\n________________________________________\n";
    print STDOUT "\nStarting test \"$test\"";
    print STDOUT "\n________________________________________\n\n";

    my $test_process = $target->CreateProcess($test);

    if (! -e $test_process->Executable ()) {
        print STDERR "Error: " . $test_process->Executable () .
                     " does not exist or is not runnable\n";
    }
    else
    {
       $test_process->Spawn ();
       $test_result = $test_process->WaitKill ($test_timeout);

       if ($test_result != 0)
       {
           print STDERR "\n________________________________________\n";
           print STDERR "\nERROR: \"$test\" returned $test_result";
           print STDERR "\n________________________________________\n";
       }
    }
    print STDOUT "\n________________________________________\n";
    print STDOUT "\n\"$test\" completed";
    print STDOUT "\n________________________________________\n";
}
