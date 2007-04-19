eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;  
 
# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", "-k file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$iorfile>\n";
	$SV->Kill ();
	exit 1;
}

$client = $CL->SpawnWaitKill (60);

$SV->Kill();

unlink $iorfile;

if ($client != 0) {
	print STDERR "ERROR: client returned $client\n";
	exit $client;
}

exit 0;
