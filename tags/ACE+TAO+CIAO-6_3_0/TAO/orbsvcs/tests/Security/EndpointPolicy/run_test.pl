eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "test.ior";
my $port = 12345;
my $test_iorfile = $test->LocalFile ($iorbase);
$test->DeleteFile($iorbase);

$T = $test->CreateProcess ("test", "-ORBdebuglevel $debug_level ".
                                   "-ORBDottedDecimalAddresses 0 ".
                                   "-ORBUseSharedProfile 1 ".
                                   "-o $test_iorfile -p $port");

print "Starting server using shared profiles\n";

$test_status = $T->Spawn ();

if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    exit 1;
}

if ($test->WaitForFileTimed ($iorbase,
                             $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_iorfile>\n";
    $T->Kill (); $T->TimedWait (1);
    exit 1;
}

# The server ought to die quickly on its own.
$test_status = $T->WaitKill ($test->ProcessStopWaitInterval());
if ($test_status != 0) {
    print STDERR "ERROR: server [single profile per IOR] returned $test_status\n";
    exit 1;
}

$test->DeleteFile($iorbase);

exit $status;
