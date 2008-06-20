# $Id$

# This module contains a few miscellanous functions and some
# startup ARGV processing that is used by all tests.

use PerlACE::Process;
use PerlACE::ConfigList;

package PerlACE;
use File::Spec;
use Cwd;

$PerlACE::ACE_ROOT = $ENV{ACE_ROOT};
if(exists $ENV{TAO_ROOT}) {
    $PerlACE::TAO_ROOT = $ENV{TAO_ROOT};
} else {
    $PerlACE::TAO_ROOT = "$PerlACE::ACE_ROOT/TAO";
}
if(exists $ENV{CIAO_ROOT}) {
    $PerlACE::CIAO_ROOT = $ENV{CIAO_ROOT};
} else {
    $PerlACE::CIAO_ROOT = "$PerlACE::TAO_ROOT/CIAO";
}

my $config = new PerlACE::ConfigList;
$PerlACE::TestConfig = $config;

# load VxWorks Process helpers in case this is a VxWorks target build
$PerlACE::Static = $config->check_config("STATIC");
$PerlACE::VxWorks_Test = $config->check_config("VxWorks");
$PerlACE::VxWorks_RTP_Test = $config->check_config("VxWorks_RTP");
if ($PerlACE::VxWorks_Test or $PerlACE::VxWorks_RTP_Test) {
    require PerlACE::ProcessVX;
}

# Load LabVIEW RT Process helpers in case this is a LabVIEW RT target build.
$PerlACE::LabVIEW_RT_Test = $config->check_config("LabVIEW_RT");
if ($PerlACE::LabVIEW_RT_Test) {
    require PerlACE::ProcessLVRT;
}

# Figure out the svc.conf extension
$svcconf_ext = $ENV{"ACE_RUNTEST_SVCCONF_EXT"};
if (!defined $svcconf_ext) {
    $svcconf_ext = ".conf";
}

# Default timeout.  NSCORBA needs more time for process start up.
$wait_interval_for_process_creation = (($PerlACE::VxWorks_Test or $PerlACE::VxWorks_RTP_Test) ? 60 : 15);
if ($^O eq 'VMS') {
  $wait_interval_for_process_creation *= 3;
}
elsif ($^O eq 'nto') {
  ## QNX can be slow to start processes
  $wait_interval_for_process_creation += 5;
}

$wait_interval_for_process_shutdown = (($PerlACE::VxWorks_Test or $PerlACE::VxWorks_RTP_Test) ? 30 : 10);

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

sub is_labview_rt_test()
{
    return ($PerlACE::LabVIEW_RT_Test);
}

sub is_vxworks_test()
{
    return ($PerlACE::VxWorks_Test || $PerlACE::VxWorks_RTP_Test);
}

sub is_vxworks_rtp_test()
{
    return ($PerlACE::VxWorks_RTP_Test);
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

  if (defined $ENV{"HOST_ROOT"}) {
    add_path('PATH', VX_HostFile ($value));
    add_path('LD_LIBRARY_PATH', VX_HostFile ($value));
    add_path('LIBPATH', VX_HostFile ($value));
    add_path('SHLIB_PATH', VX_HostFile ($value));
  }
}

sub check_privilege_group {
  if ($^O eq 'hpux') {
    my($access) = 'RTSCHED';
    my($status) = 0;
    my($getprivgrp) = '/bin/getprivgrp';

    if (-x $getprivgrp) {
      if (open(GPG, "$getprivgrp |")) {
        while(<GPG>) {
          if (index($_, $access) >= 0) {
            $status = 1;
          }
        }
        close(GPG);
      }
    }

    if (!$status) {
      print STDERR "WARNING: You must have $access privileges to run this test.\n",
                   "         Run \"man 1m setprivgrp\" for more information.\n";
      exit(0);
    }
  }
}

# Add PWD to the load library path
add_lib_path ('.');

$sleeptime = 5;

1;
