# $Id$

package PerlACE::ProcessVX;

use strict;
use Win32::Process;
use File::Basename;
use File::Spec;
use FileHandle;
use Cwd;

require Net::Telnet;

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
    $self->{REBOOT_CMD} = $ENV{"ACE_RUN_VX_REBOOT_CMD"};
    if (!defined $self->{REBOOT_CMD}) {
        $self->{REBOOT_CMD} = "reboot";
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

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
        unlink "run_vx.pl";
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
            }
            $iboot = IO::Socket::INET->new ("$ENV{'ACE_RUN_VX_IBOOT'}");
            if  ($iboot) {
              $iboot->send ("\ePASS\ef\r");
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
              $iboot->send ("\ePASS\en\r");
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
    $cwdrel =~ s/\\/\//g;
    $program = basename($program, $self->{EXE_EXT});

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

        if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
            @cmds[$cmdnr++] = 'memShow()';
        }

        if (defined $ENV{'ACE_RUN_ACE_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("ACE_DEBUG=' . $ENV{"ACE_RUN_ACE_DEBUG"} . '")';
        }

        if (defined $ENV{'ACE_RUN_TAO_ORB_DEBUG'}) {
            @cmds[$cmdnr++] = 'putenv("TAO_ORB_DEBUG=' . $ENV{"ACE_RUN_TAO_ORB_DEBUG"} . '")';
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

    print $oh "require Net::Telnet;\n";
    print $oh "my \@cmds;\n";
    print $oh "my \$prompt = '$prompt';\n";
    my $i = 0;
    while($i < $cmdnr) {
      print $oh "\@cmds[$i] = '" . @cmds[$i++] . "';\n";
    }
    print $oh "my \$cmdnr = $cmdnr;\n\n";

    print $oh <<'__END__';
if (defined $ENV{'ACE_TEST_VERBOSE'}) {
  print "$cmdline\n";
}

my $ok;
my $t = new Net::Telnet(Timeout => 600, Errmode => 'return');
$t->open($ENV{'ACE_RUN_VX_TGTHOST'});
$t->print("");
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
