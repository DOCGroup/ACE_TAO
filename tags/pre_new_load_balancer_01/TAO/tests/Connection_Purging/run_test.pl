eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$T = new PerlACE::Process ("Connection_Purging", "-ORBCollocation no");

$client = $T->SpawnWaitKill (60);

if ($client == -1) {
    exit 1;
}

exit 0;
