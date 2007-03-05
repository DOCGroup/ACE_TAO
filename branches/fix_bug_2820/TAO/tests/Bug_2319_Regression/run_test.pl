eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that tests AMH exceptions

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$AMH = new PerlACE::Process ("server", "");

# Run the AMH server.
$AMH->Spawn ();

$amhserver= $AMH->WaitKill (60);
if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

exit $status;
