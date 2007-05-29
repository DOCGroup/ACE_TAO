eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
$iorfilebase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
unlink $iorfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}

$SV->Spawn ();

## Slower hardware can require much more time to complete.
$server = $SV->WaitKill (90);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
