eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("server", "");

$SV->Spawn ();

$server = $SV->WaitKill (45);

if ($server != 0) {
    print STDERR "ERROR: a deadlock occured in server  and returned $server\n";
    $status = 1;
}
exit $status;
