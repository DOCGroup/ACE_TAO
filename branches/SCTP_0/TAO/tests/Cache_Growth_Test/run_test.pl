eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", " -k file://$iorfile");
$CL1 = new PerlACE::Process ("client", " -k file://$iorfile -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

for ($i = 0; $i != 30; $i++) {
    $client = $CL->SpawnWaitKill (300);
    
    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }
}

$client1 = $CL1->SpawnWaitKill (300);

if ($client1 != 0) {
    print STDERR "ERROR: client1 returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
