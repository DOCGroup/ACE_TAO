eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

print STDERR "================ Reference Creation Test\n";

$CO = new PerlACE::Process ("create_reference", " -i 10000");

$collocated = $CO->SpawnWaitKill (120);

if ($collocated != 0) {
    print STDERR "ERROR: collocated test returned $collocated\n";
    $status = 1;
}

exit $status;
