eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "test.ior";
my $client_iorfile = $client->LocalFile ($iorbase);

$client->DeleteFile($iorbase);

$CL = $client->CreateProcess ("client", "-k $client_iorfile");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client->WaitForFileTimed ($iorbase,
                        $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client->DeleteFile($iorbase);

exit $status;
