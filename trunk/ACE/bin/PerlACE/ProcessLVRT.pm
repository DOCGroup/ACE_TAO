# $Id$
#
# ProcessLVRT - how to run ACE+TAO tests on a LabVIEW RT target.
# Tests on LabVIEW RT are not executables - LabVIEW RT can't start plain
# executables; the tests are built/packaged as DLLs and loaded and executed
# from a DLL loaded at LabVIEW RT boot time. The DLL on the target listens
# on a TCP port (8888 by default) for connections from the host. Host requests
# actions using text commands to the target.
#
# NOTE: This module requires the Net-Telnet Perl module.
#
# We can FTP files to and from the LabVIEW target, but there's no NFS or
# SMB shares.

package PerlACE::ProcessLVRT;
our @ISA = "PerlACE::Process";

use strict;
use Cwd;
use English;
use File::Basename;
use Net::FTP;
use Net::Telnet;
use POSIX qw(:time_h);

$PerlACE::ProcessLVRT::ExeSubDir = './';

### Check for -ExeSubDir commands, store the last one
my @new_argv = ();

for(my $i = 0; $i <= $#ARGV; ++$i) {
    if ($ARGV[$i] eq '-ExeSubDir') {
        if (defined $ARGV[$i + 1]) {
            $PerlACE::ProcessLVRT::ExeSubDir = $ARGV[++$i].'/';
        }
        else {
            print STDERR "You must pass a directory with ExeSubDir\n";
            exit(1);
        }
    }
    else {
        push @new_argv, $ARGV[$i];
    }
}
@ARGV = @new_argv;

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    $self->{EXECUTABLE} = shift;
    $self->{ARGUMENTS} = shift;
    $self->{RUNNING} = 0;
    $self->{IGNOREEXESUBDIR} = 1;
    $self->{FTP} = undef;
    $self->{TARGET} = undef;
    $self->{REBOOT_CMD} = $ENV{"ACE_REBOOT_LVRT_CMD"};
    if (!defined $self->{REBOOT_CMD}) {
        $self->{REBOOT_CMD} = 'I_Need_A_Reboot_Command';
    }
    $self->{REBOOT_TIME} = $ENV{"ACE_LVRT_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 200;
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

    # Reboot if needed; set up clean for the next test.
    if ($self->{NEED_REBOOT} == 1 && $self->{REBOOT_CMD}) {
        print STDERR "Attempting to reboot target...\n";
        system ($self->{REBOOT_CMD});
        sleep ($self->{REBOOT_TIME});
    }

    if (defined $self->{FTP}) {
        $self->{FTP}->close;
    }
    if (defined $self->{TARGET}) {
        $self->{TARGET}->close;
    }
}

###############################################################################

# Adjust executable name for LabVIEW RT testing needs. These tests are DLLs.

sub Executable
{
    my $self = shift;

    if (@_ != 0) {
        $self->{EXECUTABLE} = shift;
    }

    my $executable = $self->{EXECUTABLE};

    my $basename = basename ($executable);
    my $dirname = dirname ($executable). '/';

    $executable = $dirname.$PerlACE::Process::ExeSubDir.$basename.".DLL";
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

    my $commandline = "run " . basename($self->Executable(), ".dll");
    if (defined $self->{ARGUMENTS}) {
        $commandline .= ' '.$self->{ARGUMENTS};
    }

    return $commandline;
}

###############################################################################

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
    }

    my $targethost;
    if (defined $ENV{'ACE_RUN_LVRT_TGTHOST'}) {
        $targethost = $ENV{'ACE_RUN_LVRT_TGTHOST'};
    }
    else {
        print STDERR "You must define target hostname/IP with ",
                     "ACE_RUN_LVRT_TGTHOST\n";
        return -1;
    }
    my $targetport;
    if (defined $ENV{'ACE_RUN_LVRT_TGTPORT'}) {
        $targetport = $ENV{'ACE_RUN_LVRT_TGTPORT'};
    }
    else {
        $targetport = 8888;
    }

    my $status = 0;

    my $program = $self->Executable ();
    my $cwdrel = dirname ($program);
    if (length ($cwdrel) > 0) {
        $cwdrel = File::Spec->abs2rel( cwd(), $ENV{"ACE_ROOT"} );
    }
    else {
        $cwdrel = File::Spec->abs2rel( $cwdrel, $ENV{"ACE_ROOT"} );
    }

    $self->{FTP} = new Net::FTP ($targethost);
    if (!defined $self->{FTP}) {
        print STDERR "$@\n";
        $self->{NEED_REBOOT} = 1;
        return -1;
    }
    $self->{FTP}->login("","");
    $self->{FTP}->cwd("/ni-rt");
    $self->{FTP}->binary();
    $self->{FTP}->put($program);

    $self->{TARGET} = new Net::Telnet(Timeout => 600, Errmode => 'return');
    if (!$self->{TARGET}->open(Host => $targethost, Port => $targetport)) {
        print STDERR "ERROR: target $targethost:$targetport: ",
                      $self->{TARGET}->errmsg(), "\n";
        $self->{NEED_REBOOT} = 1;
        $self->{FTP}->delete($program);
        return -1;
    }
    my $cmdline = $self->CommandLine();
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print "-> $cmdline\n";
    }
    $self->{TARGET}->print("$cmdline");
    my $reply;
    $reply = $self->{TARGET}->getline();
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print "<- $reply\n";
    }
    if ($reply eq "OK\n") {
        $self->{RUNNING} = 1;
        return 0;
    }
    print STDERR "ERROR: can't $cmdline: " . $reply . "\n";
    $self->{FTP}->delete($program);
    $self->{NEED_REBOOT} = 1;
    return -1;
}


# Wait for the process to exit or kill after a time period

sub WaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    my $status = $self->TimedWait ($timeout);

    if ($status == -1) {
        print STDERR "ERROR: $self->{EXECUTABLE} timedout\n";
        $self->{NEED_REBOOT} = 1;
        $self->Kill ();
    }

    $self->{RUNNING} = 0;

    # Now get the log file from the test, and delete the test from the target.
    # The FTP session should still be open.
    my $program = $self->Executable ();
    my $logname = basename($program,".dll") . ".log";
    $program = basename($program);
    $self->{FTP}->delete($program);
    $self->{FTP}->cwd("\\ni-rt\\system\\log");
    $self->{FTP}->get($logname,"log\\$logname");
    $self->{FTP}->delete($logname);

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
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        $self->Kill();
    }

    return $self->WaitKill ($timeout);
}

sub Kill ()
{
    my $self = shift;

    if ($self->{RUNNING}) {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print "-> kill\n";
        }
        $self->{TARGET}->print("kill");
        # Just wait for any reply; don't care what it is.
        my $reply = $self->{TARGET}->getline();
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print "<- $reply\n";
        }
    }

    $self->{RUNNING} = 0;
}

# Wait until a process exits.
# return -1 if the process is still alive.
sub Wait ($)
{
    my $self = shift;
    my $timeout = shift;
    if (!defined $timeout || $timeout < 0) {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print "-> wait\n";
        }
        $self->{TARGET}->print("wait");
        my $reply = $self->{TARGET}->getline();
        $self->{RUNNING} = 0;
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
          print "<- $reply\n";
        }
        return 0+ $reply;
    } else {
        return TimedWait($self, $timeout);
    }

}

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;
    my $reply;
    if (!$self->{RUNNING}) {
      return -1;
    }

CHECK:
    while ($timeout > 0) {
        $self->{TARGET}->print ("status");
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
          print "-> status\n";
        }
        $reply = $self->{TARGET}->getline(Timeout => $timeout);
        if (!defined $reply) {
            last CHECK;
        }
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
          print "<- $reply\n";
        }
        if ($reply =~ /^RUNNING/) {
            sleep 2;
            $timeout -= 2;
            next CHECK;
        }
        # Have a status; return it.
        $self->{RUNNING} = 0;
        return 0+ $reply;
    }

    return -1;
}

1;
