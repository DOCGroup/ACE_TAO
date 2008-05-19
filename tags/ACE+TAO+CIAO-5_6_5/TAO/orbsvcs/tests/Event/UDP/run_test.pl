eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$S1 = new PerlACE::Process ("sender",
                            "");
$R1 = new PerlACE::Process ("receiver",
                           "");
$S2 = new PerlACE::Process ("sender",
                            "-v");
$R2 = new PerlACE::Process ("receiver",
                           "-v");

print STDOUT "Starting receiver with plain text\n";
$R1->Spawn ();

sleep 1;

print STDOUT "Starting sender with plain text\n";
$S1->Spawn ();

sleep 1;

$consumer = $S1->WaitKill (150);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$receiver = $R1->WaitKill (150);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

print STDOUT "Starting receiver with valuetype\n";
$R2->Spawn ();

sleep 1;

print STDOUT "Starting sender with valuetype\n";
$S2->Spawn ();

sleep 1;

$consumer2 = $S2->WaitKill (150);

if ($consumer2 != 0) {
    print STDERR "ERROR: consumer returned $consumer2\n";
    $status = 1;
}

$receiver2 = $R2->WaitKill (150);

if ($receiver2 != 0) {
    print STDERR "ERROR: receiver returned $receiver2\n";
    $status = 1;
}

exit $status;
