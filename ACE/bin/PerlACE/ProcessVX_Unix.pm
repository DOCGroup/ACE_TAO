#! /usr/bin/perl
# $Id$

package PerlACE::ProcessVX;

use strict;
use POSIX "sys_wait_h";
use File::Basename;
use File::Spec;
use Config;
use FileHandle;
use Cwd;

eval { require Net::Telnet; };

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
    if (!defined $PerlACE::ProcessVX::RebootCmd) {
        $PerlACE::ProcessVX::RebootCmd = "reboot 0x02";
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

    # Reboot the target if necessery
    $self->reboot();

    my $program = $self->Executable ();
    my $cwdrel = dirname ($program);
    my $prjroot = defined $ENV{"ACE_RUN_VX_PRJ_ROOT"} ? $ENV{"ACE_RUN_VX_PRJ_ROOT"} : $ENV{"ACE_ROOT"};
    if (length ($cwdrel) > 0) {
        $cwdrel = File::Spec->abs2rel( cwd(), $prjroot );
    }
    else {
        $cwdrel = File::Spec->abs2rel( $cwdrel, $prjroot );
    }
    $program = basename($program, $PerlACE::ProcessVX::ExeExt);

    my @cmds;
    my $cmdnr = 0;
    my $arguments = "";
    my $prompt = '';
    my $exesubdir = defined $ENV{"ACE_RUN_VX_EXE_SUBDIR"} ? $ENV{"ACE_RUN_VX_EXE_SUBDIR"} : "";

    if (defined $ENV{"ACE_RUN_VX_STARTUP_SCRIPT"}) {
      if (defined $ENV{"ACE_RUN_VX_STARTUP_SCRIPT_ROOT"}) {
        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_STARTUP_SCRIPT_ROOT'} . '"';
      }
      @cmds[$cmdnr++] = '< ' . $ENV{"ACE_RUN_VX_STARTUP_SCRIPT"};
    }

    if ($PerlACE::VxWorks_RTP_Test) {
        @cmds[$cmdnr++] = 'cmd';
        if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $self->{SET_VX_DEFGW}) {
            @cmds[$cmdnr++] = "C mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)";
            $PerlACE::ProcessVX::VxDefGw = 0;
        }

        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . '"';
        @cmds[$cmdnr++] = 'C putenv("TMPDIR=' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '")';

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'C putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'C putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'C memShow()';
        }

        $cmdline = $program . $PerlACE::ProcessVX::ExeExt . ' ' . $self->{ARGUMENTS};
        @cmds[$cmdnr++] = $cmdline;
        $prompt = '\[vxWorks \*\]\#';
    } else {
        if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $PerlACE::ProcessVX::VxDefGw) {
            @cmds[$cmdnr++] = "mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)";
            $PerlACE::ProcessVX::VxDefGw = 0;
        }

        my(@load_commands);
        my(@unload_commands);
        if (!$PerlACE::Static && !$PerlACE::VxWorks_RTP_Test) {
          my $vxtest_file = $program . '.vxtest';
          if (handle_vxtest_file($vxtest_file, \@load_commands, \@unload_commands)) {
              @cmds[$cmdnr++] = "cd \"$ENV{'ACE_RUN_VX_TGTSVR_ROOT'}/lib\"";
              push @cmds, @load_commands;
              $cmdnr += scalar @load_commands;
          } else {
              print STDERR "ERROR: Cannot find <", $vxtest_file, ">\n";
              return -1;
          }
        }

        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . "/" . $exesubdir . '"';
        @cmds[$cmdnr++] = 'putenv("TMPDIR=' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '")';

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'memShow()';
        }

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
        }

        @cmds[$cmdnr++] = 'ld <'. $program . $PerlACE::ProcessVX::ExeExt;
        $cmdline = $program . $PerlACE::ProcessVX::ExeExt . ' ' . $self->{ARGUMENTS};
        if (defined $self->{ARGUMENTS}) {
            ($arguments = $self->{ARGUMENTS})=~ s/\"/\\\"/g;
            $arguments = ",\"" . $arguments . "\"";
        }
        @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . '"';
        @cmds[$cmdnr++] = 'ace_vx_rc = vx_execae(ace_main' . $arguments . ')';
        @cmds[$cmdnr++] = 'unld "'. $program . $PerlACE::ProcessVX::ExeExt . '"';
        push @cmds, @unload_commands;
        $cmdnr += scalar @unload_commands;
        $prompt = '->';
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

            my $telnet_port = $ENV{'ACE_RUN_VX_TGT_TELNET_PORT'};
            my $telnet_host = $ENV{'ACE_RUN_VX_TGT_TELNET_HOST'};
            if (!defined $telnet_host)  {
              $telnet_host = $ENV{'ACE_RUN_VX_TGTHOST'};
            }
            if (!defined $telnet_port)  {
                $telnet_port = 23;
              }
            my $t = new Net::Telnet(Timeout => 600, Errmode => 'return', Host => $telnet_host, Port => $telnet_port);
            $t->open();
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

            my $ok;
            $ok = $t->waitfor('/-> $/');
            if ($ok) {
              my $i = 0;
              my @lines;
              while($i < $cmdnr) {
                if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                  print @cmds[$i]."\n";
                }
                if ($t->print (@cmds[$i++])) {
                  my $blk;
                  my $buf;
                  while ($blk = $t->get) {
                    printf $blk;
                    $buf .= $blk;
                    if ($buf =~ /$prompt/) {
                      last;
                    }
                  }
                } else {
                  print $t->errmsg;
                }
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

    $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run

    return -1;
}


# Terminate the process and wait for it to finish

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        kill ('TERM', $self->{PROCESS});

        $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run
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

        $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run

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

    $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run

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

1;
