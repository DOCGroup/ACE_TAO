#! /usr/bin/perl
# $Id$

package PerlACE::ProcessVX;

use strict;
use Win32::Process;
use File::Basename;
use File::Spec;
use FileHandle;
use Cwd;

eval { require Net::Telnet; };

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
    $self->{TARGET} = shift;
    if (!defined $PerlACE::ProcessVX::WAIT_DELAY_FACTOR) {
        $PerlACE::ProcessVX::WAIT_DELAY_FACTOR = 3;
    }
    if (!defined $PerlACE::ProcessVX::RebootCmd) {
        $PerlACE::ProcessVX::RebootCmd = "reboot";
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
        unlink "run_vx.pl";
    }

    if (defined $ENV{'ACE_RUN_VX_IBOOT'} && !defined $ENV{'ACE_RUN_VX_NO_SHUTDOWN'}) {
      # Shutdown the target to save power
      $self->iboot_cycle_power(1);
    }
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
    $cwdrel =~ s/\\/\//g;
    $program = basename($program, $PerlACE::ProcessVX::ExeExt);

    unlink "run_vx.pl";
    my $oh = new FileHandle();
    if (!open($oh, ">run_vx.pl")) {
        print STDERR "ERROR: Unable to write to run_vx.pl\n";
        exit -1;
    }

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

    if (defined $ENV{"ACE_RUN_VX_STARTUP_COMMAND"}) {
      @cmds[$cmdnr++] = $ENV{"ACE_RUN_VX_STARTUP_COMMAND"};
    }

    if ($PerlACE::VxWorks_RTP_Test) {
        @cmds[$cmdnr++] = 'cmd';
        if ( defined $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} && $PerlACE::ProcessVX::VxDefGw) {
            @cmds[$cmdnr++] = "C mRouteAdd(\"0.0.0.0\", \"" . $ENV{"ACE_RUN_VX_TGTSVR_DEFGW"} . "\", 0,0,0)";
            $PerlACE::ProcessVX::VxDefGw = 0;
        }

        @cmds[$cmdnr++] = 'cd "' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '"';
        @cmds[$cmdnr++] = 'C putenv("TMPDIR=' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . '")';

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'C putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'C putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_ACE_LD_SEARCH_PATH'}) {
            @cmds[$cmdnr++] = 'C putenv("ACE_LD_SEARCH_PATH=' . $ENV{"ACE_RUN_ACE_LD_SEARCH_PATH"} . '")';
        }

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'C memShow()';
        }

        $cmdline = $program . $PerlACE::ProcessVX::ExeExt . ' ' . $self->{ARGUMENTS};
        @cmds[$cmdnr++] = $cmdline;
        $prompt = '\[vxWorks \*\]\# $';
    }
    if ($PerlACE::VxWorks_Test) {
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

        @cmds[$cmdnr++] = 'cd "' . $ENV{"ACE_RUN_VX_TGTSVR_ROOT"} . "/" . $cwdrel . "/" . $exesubdir . '"';
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

        if (defined $ENV{'ACE_RUN_ACE_LD_SEARCH_PATH'}) {
            @cmds[$cmdnr++] = 'putenv("ACE_LD_SEARCH_PATH=' . $ENV{"ACE_RUN_ACE_LD_SEARCH_PATH"} . '")';
        }

        @cmds[$cmdnr++] = 'ld <'. $program . $PerlACE::ProcessVX::ExeExt;
        if (defined $self->{ARGUMENTS}) {
            ($arguments = $self->{ARGUMENTS})=~ s/\"/\\\"/g;
            ($arguments = $self->{ARGUMENTS})=~ s/\'/\\\'/g;
            $arguments = ",\"" . $arguments . "\"";
        }
        if (defined $ENV{'ACE_RUN_VX_TGTSRV_WORKINGDIR'}) {
            @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSRV_WORKINGDIR'} . '"';
        } else {
            @cmds[$cmdnr++] = 'cd "' . $ENV{'ACE_RUN_VX_TGTSVR_ROOT'} . "/" . $cwdrel . '"';
        }
        @cmds[$cmdnr++] = 'ace_vx_rc = vx_execae(ace_main' . $arguments . ')';
        @cmds[$cmdnr++] = 'unld "'. $program . $PerlACE::ProcessVX::ExeExt . '"';
        push @cmds, @unload_commands;
        $cmdnr += scalar @unload_commands;
        $prompt = '-> $';
    }

    print $oh "require Net::Telnet;\n";
    print $oh "my \@cmds;\n";
    print $oh "my \$prompt = '$prompt';\n";
    my $i = 0;
    while($i < $cmdnr) {
      print $oh "\@cmds[$i] = '" . @cmds[$i++] . "';\n";
    }
    print $oh "my \$cmdnr = $cmdnr;\n\n";

    print $oh <<'__END__';

my $ok;
my $telnet_port = $ENV{'ACE_RUN_VX_TGT_TELNET_PORT'};
my $telnet_host = $ENV{'ACE_RUN_VX_TGT_TELNET_HOST'};
if (!defined $telnet_host)  {
  $telnet_host = $ENV{'ACE_RUN_VX_TGTHOST'};
}
if (!defined $telnet_port)  {
  $telnet_port = 23;
}
my $t = new Net::Telnet(Timeout => 600, Errmode => 'return', Host => $telnet_host, Port => $telnet_port);
if (!defined $t) {
  die "ERROR: Telnet failed to <" . $telnet_host . ":". $telnet_port . ">";
}
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
  die "ERROR: No prompt appeared\n";
}
$t->close();
sleep(2);
exit;
__END__

    close($oh);

    Win32::Process::Create ($self->{PROCESS},
                            "$^X",
                            "$^X run_vx.pl",
                            0,
                            0,
                            '.');

    Win32::Process::GetExitCode ($self->{PROCESS}, $status);

    if ($status != $STILL_ACTIVE) {
        print STDERR "ERROR: Spawn failed for <", "$^X run_vx.pl", ">\n";
        exit $status;
    }

    $self->{RUNNING} = 1;
    return 0;
}

# Wait for a process to exit with a timeout

sub TimedWait ($)
{
    my($self) = shift;
    my($timeout) = shift;
    return $self->Wait($timeout);
}


# Terminate the process and wait for it to finish

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        Win32::Process::Kill ($self->{PROCESS}, 0);
        $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run
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
        $PerlACE::ProcessVX::DoVxInit = 1; # force reboot on next run
    }
    return $result;
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


1;
