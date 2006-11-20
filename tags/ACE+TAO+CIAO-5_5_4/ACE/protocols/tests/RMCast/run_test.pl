eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;

$sender = new PerlACE::Process ("sender", "224.1.2.3:12345");
$receiver = new PerlACE::Process ("receiver", "224.1.2.3:12345");


$receiver->Spawn ();

# Wait for receiver to start.
#
sleep ($PerlACE::wait_interval_for_process_creation);

$sender->Spawn ();

$status = $receiver->WaitKill (40);

if ($status != 0) {
    print STDERR "ERROR: receiver returned $status\n";
    $status = 1;
}

$sender->Kill ();

exit $status;
