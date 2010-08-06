eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
  $SVP = new PerlACE::ProcessVX ("server", "");
}
else {
  $SVP = new PerlACE::Process ("server", "");
}
# Run the AMH server.
$sv = $SVP->Spawn ();

if ($sv != 0) {
   print STDERR "ERROR: server returned $sv\n";
   exit 1;
}

$svnk = $SVP->WaitKill (60);
if ($svnk != 0) {
    print STDERR "ERROR: Server returned $svnk\n";
    $status = 1;
}

exit $status;
