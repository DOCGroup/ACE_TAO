# $Id$

package PerlACE::Process;

use strict;
use Win32::Process;
use File::Basename;
use Cwd;

###############################################################################

# This is what GetExitCode will return if the process is still running.
my $STILL_ACTIVE = 259;

###############################################################################

### Constructor and Destructor

#
# Hack in purify support thru 2 environment variables:
#   ACE_RUN_PURIFY_CMD: complete path to purify executable
#   ACE_RUNTEST_DELAY: wait delay factor, default to 10 if
#                      ACE_RUN_PURIFY_CMD is defined, or 1 if
#                      ACE_RUN_PURIFY_CMD is not defined.
# ** Notice that when ACE_RUN_PURIFY_CMD is define, PerlACE::Process
#    reports the return status of *purify*, not the process being purified.
#
# Also hack in the ability to run the test on a WinCE device using the
# ACE_WINCE_TEST_CONTROLLER environment variable. If set, it specifies a
# controlling program to use for setting up and executing the test.
# Further setup can be specialized depending on the value of the variable.

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    $self->{RUNNING} = 0;
    $self->{IGNOREEXESUBDIR} = 0;
    $self->{PROCESS} = undef;
    $self->{EXECUTABLE} = shift;
    $self->{ARGUMENTS} = shift;
    $self->{PURIFY_CMD} = $ENV{"ACE_RUN_PURIFY_CMD"};
    $self->{PURIFY_OPT} = $ENV{"ACE_RUN_PURIFY_OPT"};
    if (!defined $PerlACE::Process::WAIT_DELAY_FACTOR) {
        if (defined $self->{PURIFY_CMD}) {
            $PerlACE::Process::WAIT_DELAY_FACTOR = 10;
        }
        else {
            $PerlACE::Process::WAIT_DELAY_FACTOR = 1;
        }
    }
    $self->{WINCE_CTL} = $ENV{"ACE_WINCE_TEST_CONTROLLER"};

    bless ($self, $class);
    return $self;
}

sub DESTROY
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        print STDERR "ERROR: <", $self->{EXECUTABLE},
                     "> still running upon object destruction\n";
        $self->Kill ();
    }
}

###############################################################################

### Some Accessors

sub Executable
{
    my $self = shift;

    if (@_ != 0) {
        $self->{EXECUTABLE} = shift;
    }

    my $executable = $self->{EXECUTABLE};

    if ($self->{IGNOREEXESUBDIR}) {
        return $executable;
    }

    my $basename = basename ($executable);
    my $dirname = dirname ($executable). '/';

    $executable = $dirname.$PerlACE::Process::ExeSubDir.$basename.".EXE";

    $executable =~ s/\//\\/g; # / <- # color coding issue in devenv

    return $executable;
}

sub Arguments
{
    my $self = shift;

    if (@_ != 0) {
        $self->{ARGUMENTS} = shift;
    }

    return $self->{ARGUMENTS};
}

sub CommandLine ()
{
    my $self = shift;

    my $commandline = $self->Executable ();

    if (defined $self->{ARGUMENTS}) {
        $commandline .= ' '.$self->{ARGUMENTS};
    }

    return $commandline;
}

sub IgnoreExeSubDir
{
    my $self = shift;

    if (@_ != 0) {
        $self->{IGNOREEXESUBDIR} = shift;
    }

    return $self->{IGNOREEXESUBDIR};
}

###############################################################################

### Spawning processes


# Spawn the process and continue.

sub Spawn ()
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                     "> already running\n";
        return -1;
    }

    if (!defined $self->{EXECUTABLE}) {
        print STDERR "ERROR: Cannot Spawn: No executable specified\n";
	    return -1;
    }

    if ($self->{IGNOREEXESUBDIR} == 0) {
        if (!-f $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> not found\n";
            return -1;
        }

        if (!-x $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> not executable\n";
            return -1;
        }
    }

    my $cmdline = "";
    my $executable = "";

    if (defined $self->{PURIFY_CMD}) {
        my $orig_cmdline = $self->CommandLine ();
        $executable = $self->{PURIFY_CMD};
        my $basename = basename ($self->{EXECUTABLE});

        my $PurifyOptions = $self->{PURIFY_OPT};
        if (!defined $PurifyOptions) {
            $PurifyOptions =
                "/run ".
#                "/save-data=$basename.pfy ".
                "/save-text-data=$basename.pfytxt ".
                "/AllocCallStackLength=20 ".
                "/ErrorCallStackLength=20 ".
                "/HandlesInUseAtExit ".
                "/InUseAtExit ".
                "/LeaksAtExit ";
        }
        my $basename = basename ($self->{EXECUTABLE});
        $cmdline =
            "purify " .
            "$PurifyOptions ".
            "$orig_cmdline" ;
    }
    elsif (defined $self->{WINCE_CTL}) {
        $executable = $self->Executable ();
        $cmdline = $self->CommandLine ();

        # Generate a script to delete the log files for this program on the
        # remote device, copy the test down to the device, run it, then
        # copy the log file(s) back to the log directory.
        unless (open (SCRIPT, ">start_test.cmd")) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> failed to create start_test.cmd\n";
            return -1;
        }

        # The device was soft-reset at the end of the test. Wait a few seconds
        # for it to come back up.
        sleep (10);

        my $testname = basename($executable,'.EXE');
        my $here = getcwd();
        $here =~ s/\//\\/g;
        $executable =~ s/^\.//;      # Chop leading .
        $executable = $here . $executable;   # Fully qualified name
        # Take off the test name from the start of the command line.
        # The command name is preprended in the script below.
        my @tokens = split(' ', $cmdline);
        @tokens = splice(@tokens,1);
        $cmdline = join(' ', @tokens);
#        print SCRIPT "del 1:\\log\\$testname*.txt\n";
        print SCRIPT "copy $executable 1:\\Windows\n";
        print SCRIPT "$testname $cmdline\n";
        if ($testname eq 'Cached_Conn_Test' || $testname eq 'Cached_Accept_Conn_Test') {
            print SCRIPT "sleep 400\n";
        }
        else {
            print SCRIPT "sleep 60\n";
        }
        print SCRIPT "copy 1:\\log\\$testname*.txt $here\\log\n";
        print SCRIPT "del 1:\\Windows\\$testname.exe\n";
        print SCRIPT "reset /s\n";
        close SCRIPT;

        $executable = $ENV{"ComSpec"};
        $cmdline = "cmd /C start /B /WAIT pocketcontroller -m NAME=start_test.cmd"
    }
    else {
        $executable = $self->Executable ();
        $cmdline = $self->CommandLine ();
    }
    Win32::Process::Create ($self->{PROCESS},
                            $executable,
                            $cmdline,
                            0,
                            0,
                            '.');

    my $status = 0;

    Win32::Process::GetExitCode ($self->{PROCESS}, $status);

    if ($status != $STILL_ACTIVE) {
        print STDERR "ERROR: Spawn failed for <", $self->CommandLine (), ">\n";
        exit $status;
    }

    $self->{RUNNING} = 1;
    return 0;
}


# Wait for the process to exit or kill after a time period

sub WaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    my $status = $self->TimedWait ($timeout);

    if ($status == -1) {
        print STDERR "ERROR: $self->{EXECUTABLE} timedout\n";
        $self->Kill ();
        # Don't need to Wait since we are on Win32
    }

    $self->{RUNNING} = 0;

    return $status;
}


# Do a Spawn and immediately WaitKill

sub SpawnWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->Spawn () == -1) {
        return -1;
    }

    return $self->WaitKill ($timeout);
}


# Kill the process

sub Kill ()
{
    my $self = shift;

    if ($self->{RUNNING}) {
        Win32::Process::Kill ($self->{PROCESS}, -1);
    }

	$self->{RUNNING} = 0;
}


# Terminate the process and wait for it to finish

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        Win32::Process::Kill ($self->{PROCESS}, 0);
    }

    return $self->WaitKill ($timeout);
}


# Wait until a process exits.

sub Wait ()
{
    my $self = shift;

    if ($self->{RUNNING}) {
        Win32::Process::Wait ($self->{PROCESS}, INFINITE);
    }

    $self->{RUNNING} = 0;
}


# Wait for a process to exit with a timeout

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;

    if (!$self->{RUNNING}) {
        return 0;
    }

    if (Win32::Process::Wait ($self->{PROCESS},
                              $timeout * 1000 * $PerlACE::Process::WAIT_DELAY_FACTOR) == 0) {
        return -1;
    }

    my $status = 0;
    Win32::Process::GetExitCode ($self->{PROCESS}, $status);

    # If this was a WinCE test, the log file was copied back to the log
    # directory but is named .txt, not .log. Rename it so the log analyzer
    # can find it.
    if (defined $self->{WINCE_CTL}) {
        my $log_name_txt;
        $log_name_txt = dirname($self->{EXECUTABLE})."\\log\\";
        $log_name_txt .= basename($self->{EXECUTABLE},'.EXE').".txt";
        if (-e $log_name_txt) {
            my $log_name = $log_name_txt;
            $log_name =~ s/\.txt$/.log/i;
            rename($log_name_txt, $log_name);
        }
    }

    return $status;
}

1;
