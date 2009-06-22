eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use strict;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

my $locior = 'imr.ior';
my $actior = 'act.ior';
my $serverior = 'server.ior';
my $initref = "-ORBInitRef ImplRepoService=file://$locior";
my $IMR_LOCATOR = new PerlACE::Process("../../../ImplRepo_Service/ImplRepo_Service",
                                       "-o $locior");
my $IMR_ACTIVATOR = new PerlACE::Process("../../../ImplRepo_Service/ImR_Activator",
                                         "-o $actior $initref");
my $TAO_IMR = new PerlACE::Process("$ENV{ACE_ROOT}/bin/tao_imr");

my $CL = new PerlACE::Process("client", "-k file://$serverior");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$TAO_IMR->IgnoreExeSubDir(1);

unlink($locior, $actior, $serverior);

my $result = $IMR_LOCATOR->Spawn();
if (PerlACE::waitforfile_timed($locior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: IMR Locator returned $result\n";
  $IMR_LOCATOR->Kill();
  exit(1);
}

$result = $IMR_ACTIVATOR->Spawn();
if (PerlACE::waitforfile_timed($actior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: IMR Activator returned $result\n";
  $IMR_ACTIVATOR->Kill();
  $IMR_LOCATOR->Kill();
  exit(1);
}

my $SV = new PerlACE::Process("server");
$TAO_IMR->Arguments($initref .
                    " add test_server -c \"" . $SV->Executable() .
                    " $initref -ORBUseIMR 1 " .
                    "-o $serverior\"");

$result = $TAO_IMR->SpawnWaitKill(15);
if ($result != 0) {
  print STDERR "ERROR: TAO IMR returned $result\n";
  $IMR_ACTIVATOR->Kill();
  $IMR_LOCATOR->Kill();
  exit(1);
}

$TAO_IMR->Arguments($initref . ' start test_server');

$result = $TAO_IMR->SpawnWaitKill(15);
if ($result != 0) {
  print STDERR "ERROR: TAO IMR returned $result\n";
  $IMR_ACTIVATOR->Kill();
  $IMR_LOCATOR->Kill();
  exit(1);
}

$result = $CL->SpawnWaitKill(15);
if ($result != 0) {
  print STDERR "ERROR: client returned $result\n";
  $IMR_ACTIVATOR->Kill();
  $IMR_LOCATOR->Kill();
  exit(1);
}

$IMR_ACTIVATOR->Kill();
$IMR_LOCATOR->Kill();
unlink($locior, $actior, $serverior);
exit(0);
