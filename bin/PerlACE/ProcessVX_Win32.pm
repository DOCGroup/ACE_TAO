# $Id$

package PerlACE::ProcessVX;

use strict;
use Win32::Process;
use File::Basename;
use File::Spec;
use FileHandle;
use Cwd;

###############################################################################

# This is what GetExitCode will return if the process is still running.
my $STILL_ACTIVE = 259;

my $set_vx_defgw = 1;
my $do_vx_init = (defined $ENV{"ACE_RUN_VX_NO_INITIAL_REBOOT"}) ? 0 : 1;

###############################################################################

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    $self->{RUNNING} = 0;
    $self->{IGNOREEXESUBDIR} = 1;
    $self->{PROCESS} = undef;
    $self->{EXECUTABLE} = shift;
    $self->{ARGUMENTS} = shift;
    if (!defined $PerlACE::ProcessVX::WAIT_DELAY_FACTOR) {
        $PerlACE::ProcessVX::WAIT_DELAY_FACTOR = 3;
    }
    $self->{WINDSH} = $ENV{"ACE_RUN_WINDSH"};
    if (!defined $self->{WINDSH}) {
        $self->{WINDSH} = $ENV{"WIND_BASE"} . "\\host\\" . $ENV{"WIND_HOST_TYPE"} . "\\bin\\windsh.exe";
    }
    $self->{REBOOT_CMD} = $ENV{"ACE_RUN_VX_REBOOT_CMD"};
    if (!defined $self->{REBOOT_CMD}) {
        $self->{REBOOT_CMD} = "reboot";
    }
    $self->{REBOOT_TIME} = $ENV{"ACE_RUN_VX_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 90;
    }

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

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
        unlink "run_test.vxs";
    }
}

###############################################################################

### Some Accessors

sub Normalize_Executable_Name
{
    my $executable = shift;

    my $basename = basename ($executable);
    my $dirname = dirname ($executable). '/';

    $executable = $dirname.$PerlACE::ProcessVX::ExeSubDir.$basename.".out";

    ## Installed executables do not conform to the ExeSubDir
    if (! -e $executable && -e $dirname.$basename.'.out') {
      $executable = $dirname.$basename.'.out';
    }

    $executable =~ s/\//\\/g; # / <- # color coding issue in devenv

    return $executable;
}


sub Executable
{
    my $self = shift;

    if (@_ != 0) {
        $self->{EXECUTABLE} = shift;
    }

    my $executable = $self->{EXECUTABLE};

    if ($self->{IGNOREEXESUBDIR} == 0) {
      $executable = PerlACE::ProcessVX::Normalize_Executable_Name ($executable);
    }
    else {
      $executable = $executable.".out";
      $executable =~ s/\//\\/g; # / <- # color coding issue in devenv
    }

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

    if (!-x $self->{WINDSH}) {
        print STDERR "ERROR: Cannot Spawn: <", $self->{WINDSH},
                     "> not executable\n";
        return -1;
    }

    if ($self->{IGNOREEXESUBDIR} == 0) {
        if (!-f $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> not found\n";
            return -1;
        }
    }

    my $status = 0;

    my $cmdline;
    ##
    ## initialize VxWorks kernel (reboot!) if needed
    if ($do_vx_init || $ENV{'ACE_RUN_VX_TGT_REBOOT'}) {
        if (defined $ENV{'ACE_RUN_VX_REBOOT_TOOL'}) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "Calling: $ENV{'ACE_RUN_VX_REBOOT_TOOL'}\n";
            }
            system ($ENV{'ACE_RUN_VX_REBOOT_TOOL'});
        }
        else {
            $cmdline = $self->{WINDSH} . " -e \"shParse {" . $self->{REBOOT_CMD} . "}\" " . $ENV{'ACE_RUN_VX_TGTSVR'};
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print $cmdline . "\n";
            }
            ## reboot VxWorks kernel to cleanup
            Win32::Process::Create ($self->{PROCESS},
                                    $self->{WINDSH},
                                    $cmdline,
                                    0,
                                    0,
                                    '.');
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "Spawned: " . $cmdline . "\n";
            }
            Win32::Process::GetExitCode ($self->{PROCESS}, $status);
            if ($status != $STILL_ACTIVE) {
                print STDERR "ERROR: Spawn failed for <", $self->{WINDSH}, ">\n";
                exit $status;
            }
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "Status: $status\n";
            }
            $self->{RUNNING} = 1;
            $status = $self->TimedWait (3);
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "TimedWait Status: $status\n";
            }
            if ($status == -1) {
                $self->Kill ();
                # Don't need to Wait since we are on Win32
            }
            $self->{RUNNING} = 0;
            $self->{PROCESS} = undef;
        }
        $set_vx_defgw = 1;
        $do_vx_init = 0;

        sleep($self->{REBOOT_TIME});
    }

    my $program = $self->Executable ();
    my $cwdrel = dirname ($program);
    if (length ($cwdrel) > 0) {
        $cwdrel = File::Spec->abs2rel( cwd(), $ENV{"ACE_ROOT"} );
    }
    else {
        $cwdrel = File::Spec->abs2rel( $cwdrel, $ENV{"ACE_ROOT"} );
    }
    $cwdrel =~ s/\\/\//g;
    $program = basename($program, ".out");

    unlink "run_test.vxs";
    my $oh = new FileHandle();
    if (!open($oh, ">run_test.vxs")) {
        print STDERR "ERROR: Unable to write to run_test.vxs\n";
        exit -1;
    }

    if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $set_vx_defgw ) {
        print $oh "\n" .
                "mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)\n";
        $set_vx_defgw = 0;
    }

    print $oh "\n" .
              "cd \"" . $ENV{"ACE_ROOT"} . "/" . $cwdrel . "\"\n" .
              "\@cd \"" . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . "\"\n" .
              "putenv(\"TMPDIR=" . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . "\")\n";

    if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
        print $oh "memShow();\n";
    }

    my $length = length ($program) + 2;
    my $arguments = "";
    if (defined $self->{ARGUMENTS}) {
        ($arguments = $self->{ARGUMENTS})=~ s/\"/\\\"/g;
        $arguments = ",\"" . $arguments . "\"";
    }
    print $oh "write(2, \"\\n$program\\n\", $length);\n" .
              "ld 1,0,\"" . $program . ".out\"\n" .
              "ace_vx_rc = vx_execae(ace_main" . $arguments . ")\n" .
              "unld \"" . $program . ".out\"\n" .
              "exit(ace_vx_rc)\n";

    close($oh);

    my $executable = $self->{WINDSH};
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        $cmdline = $self->{WINDSH} . " -s run_test.vxs " . $ENV{"ACE_RUN_VX_TGTSVR"};
        print "$executable $cmdline\n";
    }
    else {
        $cmdline = $self->{WINDSH} . " -q -s run_test.vxs " . $ENV{"ACE_RUN_VX_TGTSVR"};

    }
    Win32::Process::Create ($self->{PROCESS},
                            $executable,
                            $cmdline,
                            0,
                            0,
                            '.');

    Win32::Process::GetExitCode ($self->{PROCESS}, $status);

    if ($status != $STILL_ACTIVE) {
        print STDERR "ERROR: Spawn failed for <", $cmdline, ">\n";
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

        $do_vx_init = 1; # force reboot on next run
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
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        Win32::Process::Kill ($self->{PROCESS}, 0);
        $do_vx_init = 1; # force reboot on next run
    }

    return $self->WaitKill ($timeout);
}


# Wait until a process exits.
# return -1 if the process is still alive.

sub Wait ($)
{
    my $self = shift;
    my $timeout = shift;
    if (!defined $timeout || $timeout < 0) {
      $timeout = INFINITE;
    } else {
      $timeout = $timeout * 1000 * $PerlACE::ProcessVX::WAIT_DELAY_FACTOR;
    }

    my $result = 0;

    if ($self->{RUNNING}) {
      $result = Win32::Process::Wait ($self->{PROCESS}, $timeout);
      if ($result == 0) {
        return -1;
      }
    }
    Win32::Process::GetExitCode ($self->{PROCESS}, $result);
    if ($result != 0) {
        $do_vx_init = 1; # force reboot on next run
    }
    return $result;
}


# Wait for a process to exit with a timeout

sub TimedWait ($)
{
    my($self) = shift;
    my($timeout) = shift;
    return $self->Wait($timeout);
}

1;
