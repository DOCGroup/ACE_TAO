eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$status = 0;
$type = "";

print STDERR "\nDynamic Any struct and union alias tests\n";

$CL = new PerlACE::Process ("client");

$client = $CL->SpawnWaitKill (30);

if ($client != 0) 
{
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

exit $status;
