# $Id$

# This module contains a few miscellanous functions and some
# startup ARGV processing that is used by all tests.

# @todo Move config into Config
@CONFIGS = ();

use PerlACE::Process;
package PerlACE;

sub CheckForConfig
{
    my @new_argv = ();

    for($i = 0; $i <= $#ARGV; ++$i) {
        if ($ARGV[$i] eq '-Config') {  
            if (defined $ARGV[$i + 1]) {
                push @::CONFIGS, $ARGV[++$i];
            }
            else {
              print STDERR "You must pass a directory with Config\n";
              exit(1);
            }
        }
        else {
            push @new_argv, $ARGV[$i];
        }
    }
    @ARGV = @new_argv;
}

CheckForConfig ();



# Returns a unique id, uid for unix, last digit of IP for NT
sub uniqueid
{
  if ($^O eq "MSWin32")
  {
    my $uid = 1;

    open (IPNUM, "ipconfig|") || die "Can't run ipconfig: $!\n";

    while (<IPNUM>)
    {
      if (/Address/)
      {
        $uid = (split (/: (\d+)\.(\d+)\.(\d+)\.(\d+)/))[4];
      }
    }

    close IPNUM;

    return $uid;
  }
  else
  {
    return getpwnam (getlogin ());
  }
}

# Waits until a file exists
sub waitforfile
{
  local($file) = @_;
  sleep 1 while (!(-e $file && -s $file));
}

sub waitforfile_timed
{
  my $file = shift;
  my $maxtime = shift;
  while ($maxtime-- != 0) {
    if (-e $file && -s $file) {
      return 0;
    }
    sleep 1;
  }
  return -1;
}

$sleeptime = 5;

1;
