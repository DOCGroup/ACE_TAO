# $Id$

# This module contains a few miscellanous functions and some
# startup ARGV processing that is used by all tests.

use PerlACE::Process;
##use PerlACE::ProcessVX;
use PerlACE::ConfigList;

package PerlACE;
use File::Spec;
use Cwd;

$PerlACE::ACE_ROOT = $ENV{ACE_ROOT};
$PerlACE::TAO_ROOT;
if(exists $ENV{TAO_ROOT}) {
    $PerlACE::TAO_ROOT = $ENV{TAO_ROOT};
} else {
    $PerlACE::TAO_ROOT = "$PerlACE::ACE_ROOT/TAO";
}

my $config = new PerlACE::ConfigList;
$PerlACE::VxWorks_Test = $config->check_config("VxWorks");
$PerlACE::VxWorks_RTP_Test = $config->check_config("VxWorks_RTP");

# load VxWorks Process helpers in case this is a VxWorks target build
if ($PerlACE::VxWorks_Test or $PerlACE::VxWorks_RTP_Test) {
    require PerlACE::ProcessVX;
}

# Figure out the svc.conf extension
$svcconf_ext = $ENV{"ACE_RUNTEST_SVCCONF_EXT"};
if (!defined $svcconf_ext) {
    $svcconf_ext = ".conf";
}

# Default timeout.  NSCORBA needs more time for process start up.
$wait_interval_for_process_creation = (($^O eq "lynxos") ? 12 : (($PerlACE::VxWorks_Test or $PerlACE::VxWorks_RTP_Test) ? 60 : 10));
if ($^O == 'VMS') {
  $wait_interval_for_process_creation *= 3;
}

# Turn on autoflush
$| = 1;

sub LocalFile ($)
{
    my $file = shift;

    my $newfile = getcwd () . '/' . $file;

    if ($^O eq "MSWin32") {
        $newfile =~ s/\//\\/g;
    }
    elsif ($^O eq 'cygwin') {
        chop($newfile = `/usr/bin/cygpath -w $newfile`);
        $newfile =~ s/\\/\\\\/g;
    }

    return $newfile;
}

sub VX_HostFile($)
{
    my $file = shift;
    $file = File::Spec->rel2abs ($file);
    $file = File::Spec->abs2rel ($file, $ENV{"ACE_ROOT"});
    return $ENV{"HOST_ROOT"}."/".$file;
}

# Returns a random port within the range of 10002 - 32767
sub random_port {
  return (int(rand($$)) % 22766) + 10002;
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
    return $>;
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
  $maxtime *= (($PerlACE::VxWorks_Test || $PerlACE::VxWorks_RTP_Test) ? $PerlACE::ProcessVX::WAIT_DELAY_FACTOR : $PerlACE::Process::WAIT_DELAY_FACTOR);

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
      print STDERR "File <$flist[$cntr]> exists but should be cleaned up\n";
    }
    unlink @flist;
  }
}

sub generate_test_file
{
  my $file = shift;
  my $size = shift;

  while ( -e $file ) {
    $file = $file."X";
  }

  my $data = "abcdefghijklmnopqrstuvwxyz";
  $data = $data.uc($data)."0123456789";

  open( INPUT, "> $file" ) || die( "can't create input file: $file" );
  for($i=62; $i < $size ; $i += 62 ) {
    print INPUT $data;
  }
  $i -= 62;
  if ($i < $size) {
    print INPUT substr($data, 0, $size-$i);
  }
  close(INPUT);

  return $file;
}

sub is_vxworks_test()
{
    return ($PerlACE::VxWorks_Test || $PerlACE::VxWorks_RTP_Test);
}

sub add_path {
  my $name   = shift;
  my $value  = shift;
  if (defined $ENV{$name}) {
    $ENV{$name} .= ($^O eq 'MSWin32' ? ';' : ':') . $value
  }
  else {
    $ENV{$name} = $value;
  }
}

sub add_lib_path {
  my($value) = shift;

  # Set the library path supporting various platforms.
  add_path('PATH', $value);
  add_path('LD_LIBRARY_PATH', $value);
  add_path('LIBPATH', $value);
  add_path('SHLIB_PATH', $value);

}

$sleeptime = 5;

1;
