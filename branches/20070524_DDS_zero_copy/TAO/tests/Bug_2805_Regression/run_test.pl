eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$debug_level = '0';
$iterations = '1';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '1';
    }
}

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("client", "-ORBdebuglevel $debug_level");
}
else {
    $CL = new PerlACE::Process ("client", "-ORBdebuglevel $debug_level");
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    exit 1;
}

exit 0;

