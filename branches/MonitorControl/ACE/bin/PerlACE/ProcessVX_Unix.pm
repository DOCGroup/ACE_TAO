# $Id$

package PerlACE::ProcessVX;

use strict;
use POSIX "sys_wait_h";
use File::Basename;
use File::Spec;
use Config;
use FileHandle;
use Cwd;

require Net::Telnet;

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
        $PerlACE::ProcessVX::WAIT_DELAY_FACTOR = 2;
    }
    $self->{REBOOT_CMD} = $ENV{"ACE_RUN_VX_REBOOT_CMD"};
    if (!defined $self->{REBOOT_CMD}) {
        $self->{REBOOT_CMD} = "reboot 0x02";
    }
    $self->{REBOOT_TIME} = $ENV{"ACE_RUN_VX_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 90;
    }
    if ($PerlACE::VxWorks_RTP_Test) {
        $self->{EXE_EXT} = '.vxe';
    } else {
        $self->{EXE_EXT} = '.out';
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
}

###############################################################################

### Some Accessors

sub Normalize_Executable_Name
{
    my $self = shift;
    my $executable = shift;

    my $basename = basename ($executable);
    my $dirname = dirname ($executable). '/';

    $executable = $dirname.$PerlACE::ProcessVX::ExeSubDir.$basename.$self->{EXE_EXT};

    ## Installed executables do not conform to the ExeSubDir
    if (! -e $executable && -e $dirname.$basename.$self->{EXE_EXT}) {
      $executable = $dirname.$basename.$self->{EXE_EXT};
    }

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
      $executable = $self->Normalize_Executable_Name ($executable);
    }
    else {
      $executable = $executable.$self->{EXE_EXT};
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

    if ($self->{IGNOREEXESUBDIR} == 0) {
        if (!-f $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> not found\n";
            return -1;
        }
    }

    my $status = 0;

    my $cmdline;

    my $t;
    my $ok;
    my $iboot;
    my $text;
    my $target_login = $ENV{'ACE_RUN_VX_LOGIN'};
    my $target_password = $ENV{'ACE_RUN_VX_PASSWORD'};

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
          if (defined $ENV{'ACE_RUN_VX_IBOOT'}) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
              print "Using iBoot: $ENV{'ACE_RUN_VX_IBOOT'}\n";

              if (defined $ENV{'ACE_RUN_VX_IBOOT_OUTLET'}) {
                print "Using iBoot Outlet #: $ENV{'ACE_RUN_VX_IBOOT_OUTLET'}\n";
              }              
            }
            $iboot = IO::Socket::INET->new ("$ENV{'ACE_RUN_VX_IBOOT'}");
            if  ($iboot) {
              # if ACE_RUN_VX_IBOOT_OUTLET is defined, we're using
              # the iBootBar, and we're using the iPAL Protocol
              # to communicate with the iBootBar
              if (defined $ENV{'ACE_RUN_VX_IBOOT_OUTLET'}) {
                $iboot->send ("\ePASS\e$ENV{'ACE_RUN_VX_IBOOT_OUTLET'}E");
              }
              else  {
                $iboot->send ("\ePASS\ef\r");
              }
              $iboot->recv ($text,128);
              if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "iBoot is currently: $text\n";
              }
              close $iboot;
            }
            else {
              print "ERROR: FAILED to execute 'reboot' command!\n";
            }
            $iboot = IO::Socket::INET->new ("$ENV{'ACE_RUN_VX_IBOOT'}");
            if  ($iboot) {
              # if ACE_RUN_VX_IBOOT_OUTLET is defined, we're using
              # the iBootBar, and we're using the iPAL Protocol
              # to communicate with the iBootBar
              if (defined $ENV{'ACE_RUN_VX_IBOOT_OUTLET'}) {
                $iboot->send ("\ePASS\e$ENV{'ACE_RUN_VX_IBOOT_OUTLET'}D");
              }
              else  {
                $iboot->send ("\ePASS\en\r");
              }
              $iboot->recv ($text,128);
              if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print "iBoot is currently: $text\n";
              }
              close $iboot;
            }
            else {
              print "ERROR: FAILED to execute 'reboot' command!\n";
            }
          }
          else {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
              print "Executing 'reboot' command over Telnet to ".$ENV{'ACE_RUN_VX_TGTHOST'}.".\n";
            }
            $t = new Net::Telnet(Timeout => 10,
                                 Prompt => '/-> $/',
                                 Errmode => 'return');
            $t->open($ENV{'ACE_RUN_VX_TGTHOST'});
            $t->print("");

            if (defined $target_login)  {
              $t->waitfor('/VxWorks login: $/');
              $t->print("$target_login");
            }

            if (defined $target_password)  {
              $t->waitfor('/Password: $/');
              $t->print("$target_password");
            }

            $ok = $t->waitfor('/-> $/');
            if ($ok) {
              $t->print($self->{REBOOT_CMD});
            }
            else {
              print "ERROR: FAILED to execute 'reboot' command!\n";
            }
            $t->close();
          }
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
    $program = basename($program, $self->{EXE_EXT});

    my @cmds;
    my $cmdnr = 0;
    my $arguments = "";
    my $prompt = '';

    if ($PerlACE::VxWorks_RTP_Test) {
        @cmds[$cmdnr++] = 'cmd';
        if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $set_vx_defgw ) {
            @cmds[$cmdnr++] = "C mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)";
            $set_vx_defgw = 0;
        }

        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . '"';
        @cmds[$cmdnr++] = 'C putenv("TMPDIR=' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '")';

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'C memShow()';
        }

        $cmdline = $program . $self->{EXE_EXT} . ' ' . $self->{ARGUMENTS};
        @cmds[$cmdnr++] = $cmdline;
        $prompt = '/\[vxWorks \*]# $/';
    } else {
        if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $set_vx_defgw ) {
            @cmds[$cmdnr++] = "mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)";
            $set_vx_defgw = 0;
        }

        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . '"';
        @cmds[$cmdnr++] = 'putenv("TMPDIR=' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '")';

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'memShow()';
        }

        @cmds[$cmdnr++] = 'ld <'. $program . $self->{EXE_EXT};
        $cmdline = $program . $self->{EXE_EXT} . ' ' . $self->{ARGUMENTS};
        if (defined $self->{ARGUMENTS}) {
            ($arguments = $self->{ARGUMENTS})=~ s/\"/\\\"/g;
            $arguments = ",\"" . $arguments . "\"";
        }
        @cmds[$cmdnr++] = 'ace_vx_rc = vx_execae(ace_main' . $arguments . ')';
        @cmds[$cmdnr++] = 'unld "'. $program . $self->{EXE_EXT} . '"';
        $prompt = '/-> $/';
    }

    FORK:
    {
        if ($self->{PROCESS} = fork) {
            #parent here
            bless $self;
        }
        elsif (defined $self->{PROCESS}) {
            #child here
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
              print "$cmdline\n";
            }

            $t = new Net::Telnet(Timeout => 600, Errmode => 'return');
            $t->open($ENV{'ACE_RUN_VX_TGTHOST'});
            $t->print("");

            my $target_login = $ENV{'ACE_RUN_VX_LOGIN'};
            my $target_password = $ENV{'ACE_RUN_VX_PASSWORD'};

            if (defined $target_login)  {
              $t->waitfor('/VxWorks login: $/');
              $t->print("$target_login");
            }

            if (defined $target_password)  {
              $t->waitfor('/Password: $/');
              $t->print("$target_password");
            }

            $ok = $t->waitfor('/-> $/');
            if ($ok) {
              $t->prompt ($prompt);
              my $i = 0;
              my @lines;
              while($i < $cmdnr) {
                if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                  print @cmds[$i]."\n";
                }
                @lines = $t->cmd (@cmds[$i++]);
                print @lines;
              }
            }
            else {
              die "ERROR: exec failed for <" . $cmdline . ">";
            }
            $t->close();
            sleep(2);
            exit;
        }
        elsif ($! =~ /No more process/) {
            #EAGAIN, supposedly recoverable fork error
            sleep 5;
            redo FORK;
        }
        else {
            # weird fork error
            print STDERR "ERROR: Can't fork <" . $cmdline . ">: $!\n";
        }
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

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        kill ('TERM', $self->{PROCESS});

        $do_vx_init = 1; # force reboot on next run
    }

    return $self->WaitKill ($timeout);
}

# really only for internal use
sub check_return_value ($)
{
    my $self = shift;
    my $rc = shift;

    my $CC_MASK = 0xff00;

    # Exit code processing
    if ($rc == 0) {
        return 0;
    }
    elsif ($rc == $CC_MASK) {
        print STDERR "ERROR: <", $self->{EXECUTABLE},
                     "> failed: $!\n";

        $do_vx_init = 1; # force reboot on next run

        return ($rc >> 8);
    }
    elsif (($rc & 0xff) == 0) {
        $rc >>= 8;
        return $rc;
    }

    # Remember Core dump flag
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

    $do_vx_init = 1; # force reboot on next run

    return 0;
}

sub Kill ()
{
    my $self = shift;

    if ($self->{RUNNING} && !defined $ENV{'ACE_TEST_WINDOW'}) {
        kill ('KILL', $self->{PROCESS});
        waitpid ($self->{PROCESS}, 0);
        $self->check_return_value ($?);
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
      waitpid ($self->{PROCESS}, 0);
    } else {
      return TimedWait($self, $timeout);
    }

}

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
      $timeout *= $PerlACE::Process::WAIT_DELAY_FACTOR;
    }

    while ($timeout-- != 0) {
        my $pid = waitpid ($self->{PROCESS}, &WNOHANG);
        if ($pid != 0 && $? != -1) {
            return $self->check_return_value ($?);
        }
        sleep 1;
    }

    $do_vx_init = 1; # force reboot on next run

    return -1;
}

1;
