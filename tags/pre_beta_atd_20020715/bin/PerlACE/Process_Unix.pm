# $Id$

package PerlACE::Process;

use strict;
use POSIX "sys_wait_h";
use Cwd;
use File::Basename;
use Config;

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

###  Grab signal names

my @signame;

if (defined $Config{sig_name}) {
    my $i = 0;
    foreach my $name (split (' ', $Config{sig_name})) {
        $signame[$i] = $name;
        $i++;
    }
}
else {
    my $i;
    for ($i = 0; $i < 255; ++$i) {
        $signame[$i] = $i;
    }
}

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

        if (!$PerlACE::Process::chorus && !-x $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (), 
                         "> not executable\n";
            return -1;
        }
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

# really only for internal use
sub check_return_value ($)
{
    my $self = shift;
    my $rc = shift;

    if ($rc == 0) {
        return 0;
    }
    elsif ($rc == 0xff00) {
        print STDERR "ERROR: <", $self->{EXECUTABLE},
                     "> failed: $!\n";
        return ($rc >> 8);
    }
    elsif (($rc & 0xff) == 0) {
        $rc >>= 8;
        return $rc;
    }

    my $dump = 0;

    if ($rc & 0x80) {
        $rc &= ~0x80;
        $dump = 1;
    }

    # check for ABRT, KILL or TERM
    if ($rc == 6 || $rc == 9 || $rc == 15) {
        return 0;
    }

    print STDERR "ERROR: <", $self->{EXECUTABLE},
                 "> exited with ";

    print STDERR "coredump from " if ($dump == 1);

    print STDERR "signal $rc : ", $signame[$rc], "\n";

    return 0;
}

sub Kill ()
{
    my $self = shift;
  
    if ($self->{RUNNING}) {
        kill ('KILL', $self->{PROCESS});
        waitpid ($self->{PROCESS}, 0);
        $self->check_return_value ($?);
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
            return $self->check_return_value ($?);
        }            
        sleep 1;
    }

    return -1;
}

1;

