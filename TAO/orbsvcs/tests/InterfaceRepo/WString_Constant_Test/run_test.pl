eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $iorbase = "if_repo.ior";
my $iorfile = $test->LocalFile ($iorbase);
$test->DeleteFile ($iorbase);

my $service = $test->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/IFR_Service/tao_ifr_service",
                                    "-o $iorfile");
my $tao_ifr = $^O eq "MSWin32"
  ? "$ENV{ACE_ROOT}/bin/tao_ifr"
  : "$ENV{TAO_ROOT}/orbsvcs/IFR_Service/tao_ifr";
my $import = $test->CreateProcess ($tao_ifr,
                                   "idl/WString_Constant_Test.idl " .
                                   "-ORBInitRef InterfaceRepository=file://$iorfile");
my $client = $test->CreateProcess ("WString_Constant_Test",
                                   "-ORBInitRef InterfaceRepository=file://$iorfile");

my $status = $service->Spawn ();
if ($status != 0)
  {
    print STDERR "ERROR: service returned $status\n";
    exit 1;
  }

if ($test->WaitForFileTimed ($iorbase, $test->ProcessStartWaitInterval ()) == -1)
  {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $service->Kill ();
    $service->TimedWait (1);
    exit 1;
  }

$status = $import->SpawnWaitKill ($test->ProcessStartWaitInterval ());
if ($status != 0)
  {
    print STDERR "ERROR: tao_ifr returned $status\n";
    $service->Kill ();
    $service->TimedWait (1);
    exit 1;
  }

$status = $client->SpawnWaitKill ($test->ProcessStartWaitInterval ());
if ($status != 0)
  {
    print STDERR "ERROR: WString_Constant_Test returned $status\n";
    $service->Kill ();
    $service->TimedWait (1);
    exit 1;
  }

$status = $service->TerminateWaitKill ($test->ProcessStopWaitInterval ());
$test->DeleteFile ($iorbase);
exit $status;
