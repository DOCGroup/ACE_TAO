# $Id$

package PerlACE::Process;

use strict;
use Win32::Process;
use File::Basename;

###############################################################################

# This is what GetExitCode will return if the process is still running.
my $STILL_ACTIVE = 259;

###############################################################################

### Constructor and Destructor

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
}


# Wait for a process to exit with a timeout

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;

    if (!$self->{RUNNING}) {
        return 0;
    }

    if (Win32::Process::Wait ($self->{PROCESS}, $timeout * 1000) == 0) {
        return -1;
    }

    my $status = 0;
    Win32::Process::GetExitCode ($self->{PROCESS}, $status);
    return $status;
}

1;
