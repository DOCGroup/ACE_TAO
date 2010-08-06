eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfilebase = "ifr.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
unlink $iorfile;

print STDERR "\n\n==== Running Forward Declared ValueType Definition test\n";

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("../../../IFR_Service/IFR_Service", "-o $iorfilebase");
}
else {
    $SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service", "-o $iorfile");
}

print STDERR "Starting IFR Service\n";
$ifspawn = $SV->Spawn ();
if ($ifspawn != 0) {
    print STDERR "ERROR: Can't spawn IFR Service $ifspawn\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile,
     $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$LOADER = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr",
                                "-ORBInitRef InterfaceRepository=file://$iorfile test.idl");
print STDERR "Loading test.idl into IFR service\n";
$status = $LOADER->SpawnWaitKill (15);
if ($status != 0) {
   print STDERR "ERROR: IFR Loading returned $status\n";
}

print STDERR "Running test_idl\n";
$TEST = new PerlACE::Process ("test_idl", "-i file://$iorfile");
$status = $TEST->SpawnWaitKill (15);
if ($status != 0) {
   print STDERR "ERROR: test_idl returned $status\n";
}

print STDERR "Stopping IFR\n";
$SV->Kill(); $SV->TimedWait (1);
unlink $iorfile;

exit $status;
