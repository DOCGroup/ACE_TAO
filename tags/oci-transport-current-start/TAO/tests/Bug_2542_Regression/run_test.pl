eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("client");
}
else {
    $SV = new PerlACE::Process ("client");    
}

$SV->Spawn ();

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: client returned $server\n";
    $status = 1;
}

exit $status;
