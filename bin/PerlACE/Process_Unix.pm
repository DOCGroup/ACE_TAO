# $Id$

package PerlACE::Process;

use strict;
use POSIX "sys_wait_h";
use Cwd;
use File::Basename;

###############################################################################

### Chorus stuff

$PerlACE::Process::chorushostname = "localhost";
$PerlACE::Process::chorus = 0;

$PerlACE::Process::cwd = getcwd();

for(my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq '-chorus') {  
        if (defined $ARGV[$i + 1]) {
            $PerlACE::Process::chorus = 1;
            $PerlACE::Process::chorushostname = $ARGV[$1 + 1];
        }
        else {
            print STDERR "The -chorus option requires " .
                         "the hostname of the target\n";
            exit(1);
        }
    
        splice(@ARGV, $i, 2);
        # Don't break from the loop just in case there
        # is an accidental duplication of the -chorus option
    }
}

###############################################################################

### Constructor

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

    $executable = $dirname.$PerlACE::Process::ExeSubDir.$basename;

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

    if ($PerlACE::Process::chorus == 1) {
        $commandline = "rsh " 
                       . $PerlACE::Process::chorushostname 
                       . " arun "
                       . $PerlACE::Process::cwd 
                       . "/"
                       . $commandline;
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

# Spawn the process and continue;

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

    if (!-f $self->{EXECUTABLE}) {
        print STDERR "ERROR: Cannot Spawn: $self->{EXECUTABLE} not found\n";
	    return -1;
    }

    if (!-x $self->{EXECUTABLE}) {
        print STDERR "ERROR: Cannot Spawn: $self->{EXECUTABLE} not executable\n";
	    return -1;
    }

    FORK:
    {
        if ($self->{PROCESS} = fork) {
            #parent here
            bless $self;
        }
        elsif (defined $self->{PROCESS}) {
            #child here
            exec $self->CommandLine ();
            die "ERROR: exec failed for <" . $self->CommandLine () . ">";
        }
        elsif ($! =~ /No more process/) {
            #EAGAIN, supposedly recoverable fork error
            sleep 5;
            redo FORK;
        }
        else {
            # weird fork error
            print STDERR "ERROR: Can't fork <" . $self->CommandLine () . ">: $!\n";
        }
    }
    $self->{RUNNING} = 1;
}

sub WaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    my $status = $self->TimedWait ($timeout);

    if ($status == -1) {
        print STDERR "ERROR: $self->{EXECUTABLE} timedout\n";
        $self->Kill (); 
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

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;
  
    if ($self->{RUNNING}) {
        kill ('TERM', $self->{PROCESS});
    }
    
    return $self->WaitKill ($timeout);
}

sub Kill ()
{
    my $self = shift;
  
    if ($self->{RUNNING}) {
        kill ('KILL', $self->{PROCESS});
        waitpid ($self->{PROCESS}, 0);
    }

    $self->{RUNNING} = 0;
}

sub Wait ()
{
    my $self = shift;
    
    waitpid ($self->{PROCESS}, 0);
}

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;
    
    while ($timeout-- != 0) {
        my $pid = waitpid ($self->{PROCESS}, &WNOHANG);
        if ($pid != 0 && $? != -1) {
            return $?;
        }
        sleep 1;
    }

    return -1;
}

1;
