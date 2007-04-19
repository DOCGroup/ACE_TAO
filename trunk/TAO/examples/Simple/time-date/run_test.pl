eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = "ior";
$conf = PerlACE::LocalFile ("client" . "$PerlACE::svcconf_ext");

# Remove the file before starting the test.
unlink $iorfile;

sub add_path {
  my($name)  = shift;
  my($value) = shift;
  if (defined $ENV{$name}) {
    $ENV{$name} .= ':' . $value
  }
  else {
    $ENV{$name} = $value;
  }
}

# Set the library path for the client to be able to load
# the Time_Date library.
add_path('LD_LIBRARY_PATH', '.');
add_path('LIBPATH', '.');
add_path('SHLIB_PATH', '.');

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client", "-f $iorfile -ORBSvcConf $conf");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$SV->Kill ();

unlink $iorfile;

exit $status;
