# $Id$

# This module contains a few miscellanous functions and some
# startup ARGV processing that is used by all tests.

use PerlACE::Process;
use PerlACE::ConfigList;

package PerlACE;
use Cwd;

# Turn on autoflush
$| = 1;

sub LocalFile ($)
{
    my $file = shift;

    my $newfile = getcwd () . '/' . $file;

    if ($^O eq "MSWin32") {
        $newfile =~ s/\//\\/g;
    }

    return $newfile;
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

sub check_n_cleanup_files
{
  my $file = shift;
  my @flist = glob ($file);

  my $cntr = 0;
  my $nfile = scalar(@flist);

  if ($nfile != 0) {
    for (; $cntr < $nfile; $cntr++) {
      print STDERR "ERROR: File <$flist[$cntr]> exists but should be cleaned up\n";
    }
    unlink @flist;
  }
}

$sleeptime = 5;

1;
