# $Id$

require Process;
$EXEPREFIX = ".".$DIR_SEPARATOR;

package ACE;

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
  sleep 1 while (!(-e $file));
}

sub waitforfile_timed
{
  my $file = shift;
  my $maxtime = shift;
  while ($maxtime-- != 0) {
    if (-e $file) {
      return 0;
    }
    sleep 1;
  }
  return -1;
}

$sleeptime = 5;

1;
