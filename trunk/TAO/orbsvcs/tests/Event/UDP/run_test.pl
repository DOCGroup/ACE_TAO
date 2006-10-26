eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

foreach $i (@ARGV) {
    if ($i eq '-v') {
        $valuetype = '-v';
    }
}
$S = new PerlACE::Process ("sender",
                           "$valuetype");
$R = new PerlACE::Process ("receiver",
                           "$valuetype");

print STDOUT "Starting sender\n";
$S->Spawn ();

sleep 1;

print STDOUT "Starting receiver\n";
$R->Spawn ();

sleep 1;

$consumer = $S->WaitKill (150);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$receiver = $R->WaitKill (150);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

exit $status;
