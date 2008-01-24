# $Id$

package PerlACE::ProcessVX;

use strict;
use English;
use POSIX qw(:time_h);

$PerlACE::ProcessVX::ExeSubDir = './';
$PerlACE::ProcessVX::DoVxInit = (defined $ENV{"ACE_RUN_VX_NO_INITIAL_REBOOT"}) ? 0 : 1;
$PerlACE::ProcessVX::VxDefGw = 1;
$PerlACE::ProcessVX::RebootTime = (defined $ENV{"ACE_RUN_VX_REBOOT_TIME"}) ? $ENV{"ACE_RUN_VX_REBOOT_TIME"} : 90;
$PerlACE::ProcessVX::ExeExt = ($PerlACE::VxWorks_RTP_Test) ? '.vxe' : '.out';
$PerlACE::ProcessVX::RebootCmd = $ENV{"ACE_RUN_VX_REBOOT_CMD"};

sub delay_factor {
  my($lps)    = 128;
  my($factor) = 1;

  ## Keep increasing the loops per second until the amount of time
  ## exceeds the number of clocks per second.  The original code
  ## did not multiply $ticks by 8 but, for faster machines, it doesn't
  ## seem to return false values.  The multiplication is done to minimize
  ## the amount of time it takes to determine the correct factor.
  while(($lps <<= 1)) {
    my($ticks) = clock();
    for(my $i = $lps; $i >= 0; $i--) {
    }
    $ticks = clock() - $ticks;
    if ($ticks * 8 >= CLOCKS_PER_SEC) {
      $factor = 500000 / (($lps / $ticks) * CLOCKS_PER_SEC);
      last;
    }
  }

  return $factor;
}

sub reboot {
  my $self = shift;
  my $iboot;
  my $text;
  my $t;
  my $ok;

  my $target_login = $ENV{'ACE_RUN_VX_LOGIN'};
  my $target_password = $ENV{'ACE_RUN_VX_PASSWORD'};

  ##
  ## initialize VxWorks kernel (reboot!) if needed
  if ($PerlACE::ProcessVX::DoVxInit || $ENV{'ACE_RUN_VX_TGT_REBOOT'}) {
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
            $t->print($PerlACE::ProcessVX::RebootCmd);
          }
          else {
            print "ERROR: FAILED to execute 'reboot' command!\n";
          }
          $t->close();
        }
      }
      $PerlACE::ProcessVX::VxDefGw = 1;
      $PerlACE::ProcessVX::DoVxInit = 0;
      sleep($PerlACE::ProcessVX::RebootTime);
  }
}

### Check for -ExeSubDir commands, store the last one
my @new_argv = ();

for(my $i = 0; $i <= $#ARGV; ++$i) {
    if ($ARGV[$i] eq '-ExeSubDir') {
        if (defined $ARGV[$i + 1]) {
            $PerlACE::ProcessVX::ExeSubDir = $ARGV[++$i].'/';
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

$PerlACE::ProcessVX::WAIT_DELAY_FACTOR = $ENV{"ACE_RUNTEST_DELAY"};

if ($OSNAME eq "MSWin32") {
    require PerlACE::ProcessVX_Win32;
}
else {
    require PerlACE::ProcessVX_Unix;
}

1;
