# $Id$

package PerlACE::Process;

use strict;
use Win32::Process;
use File::Basename;

###############################################################################

# This is what GetExitCode will return if the process is still running.
my $STILL_ACTIVE = 259;

###############################################################################

### Constructor

sub new  
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};
    
    $self->{RUNNING} = 0;
    $self->{PROCESS} = undef;
    $self->{EXECUTABLE} = shift;
    $self->{ARGUMENTS} = shift;
    
    bless ($self, $class);
    return $self;
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

    my $basename = basename ($executable);
    my $dirname = dirname ($executable). '/';

    $executable = $dirname.$PerlACE::Process::ExeSubDir.$basename.".EXE";

    $executable =~ s/\//\\/g;

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

###############################################################################

### Spawning processes


# Spawn the process and continue.

sub Spawn ()
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        print STDERR "ERROR: Cannot Spawn: <$self->{EXECUTABLE}> ",
                     "already running\n";
	return -1;
    }

    if (!defined $self->{EXECUTABLE}) {
        print STDERR "ERROR: Cannot Spawn: No executable specified\n";
	    return -1;
    }

    Win32::Process::Create ($self->{PROCESS}, 
                            $self->Executable (),
                            $self->CommandLine (), 
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
}


# Wait for the process to exit or kill after a time period

sub WaitKill ($)
{
    my $self = shift;
    my $maxtime = shift;

    my $status = $self->TimedWait ($maxtime);

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
    my $maxtime = shift;

    if ($self->Spawn () == -1) {
        return -1;
    }

    return $self->WaitKill ($maxtime);
}


# Kill the process

sub Kill ()
{
    my $self = shift;
    Win32::Process::Kill ($self->{PROCESS}, -1);
	$self->{RUNNING} = 0;
}


# Termnate the process

sub Terminate ()
{
    my $self = shift;
    Win32::Process::Kill ($self->{PROCESS}, -1);
	$self->{RUNNING} = 0;
}


# Wait until a process exits.

sub Wait ()
{
    my $self = shift;

    if ($self->{RUNNING}) {
        Win32::Process::Wait ($self->{PROCESS}, INFINITE);
    }
}


# Wait for a process to exit with a timeout

sub TimedWait
{
    my $self = shift;
    my $maxtime = shift;

    if (!$self->{RUNNING}) {
        return 0;
    }

    if (Win32::Process::Wait ($self->{PROCESS}, $maxtime * 1000) == 0) {
        return -1;
    }

    my $status = 0;
    Win32::Process::GetExitCode ($self->{PROCESS}, $status);
    return $status;
}

1;
