eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$svfile = PerlACE::LocalFile ("server.ior");
$gwfile = PerlACE::LocalFile ("gateway.ior");

unlink $svfile;
unlink $gwfile;

$status = 0;

$SV = new PerlACE::Process ("server", "-o $svfile");
$GW = new PerlACE::Process ("gateway", "-k file://$svfile -o $gwfile");
$CL = new PerlACE::Process ("client", "-k file://$gwfile -u");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($svfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$svfile>\n";
    $SV->Kill ();
    exit 1;
}

$GW->Spawn ();

if (PerlACE::waitforfile_timed ($gwfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$gwfile>\n";
    $SV->Kill ();
    $GW->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$CL = new PerlACE::Process ("client", "-k file://$gwfile -s");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->Kill ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$gateway = $GW->Kill ();

if ($gateway != 0) {
    print STDERR "ERROR: gateway returned $gateway\n";
    $status = 1;
}

unlink $svfile;
unlink $gwfile;

exit $status;
