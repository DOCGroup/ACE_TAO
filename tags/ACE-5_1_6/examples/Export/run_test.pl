eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../bin";

require ACEutils;
use Cwd;

$cwd = getcwd();
$test_exe = "$cwd$DIR_SEPARATOR"."test".$EXE_EXT;

$TEST = Process::Create ($test_exe, "");

$status = $TEST->TimedWait (20);
if ($status == -1) {
    print STDERR "ERROR: test timed out\n";
    $status = 1;
    $TEST->Kill (); $TEST->TimedWait (1);
}
elsif ($status > 0) {
    print STDERR "ERROR: test failed\n";
}

exit $status;
