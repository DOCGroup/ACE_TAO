eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

# This perl script run the Hello World component using the
# Simple Component Server

use lib "../../../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("ior");
unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("../../../tools/Simple_Component_Server/Simple_Component_Server",
                            "-i config -o $iorfile");

$CL = new PerlACE::Process ("simple_client", "-i file://$iorfile -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
