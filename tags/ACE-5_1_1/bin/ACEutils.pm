# $Id$

require Process;
$EXEPREFIX = ".".$DIR_SEPARATOR;
$TARGETHOSTNAME = "localhost";

package ACE;

sub checkForTarget
{
  my($cwd) = shift;

  for($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq '-chorus') {  
      if (defined $ARGV[$i + 1]) {
        $::TARGETHOSTNAME = $ARGV[$i + 1];
        $::EXEPREFIX = "rsh $::TARGETHOSTNAME arun $cwd$::DIR_SEPARATOR";
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
}


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
