eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Compare;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $pass = 3;
my $port = $test->RandomPort();
my $loadtest_xml = "loadtest.xml";
my $abc_xml = "abc.xml";
my $prefix = "abc";

$T = $test->CreateProcess ("main", "-ORBdebuglevel $debug_level -orbobjrefstyle url ".
                                   "-ORBEndpoint iiop://:$port -pass $pass");

sub cleanup() {
    $test->DeleteFile($loadtest_xml);
    $test->DeleteFile($abc_xml);

    for ($i = 0; $i < 10; ++$i) {
        $test->DeleteFile($prefix.".00".$i);
    }
    for ($i = 10; $i < 20; ++$i) {
        $test->DeleteFile($prefix.".0".$i);
    }
}

cleanup();

$test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval());

if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    $status = 1;
}

$different = compare($loadtest_xml, $abc_xml);
if ($different) {
    print "ERROR : $loadtest_xml != $abc_xml\n";
    exit 1;
}
else {
    print "Success : $loadtest_xml == $abc_xml\n";
}

exit $status;
