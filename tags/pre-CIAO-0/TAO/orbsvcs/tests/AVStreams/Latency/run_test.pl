eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

# amount of delay between running the servers

$status = 0;

$pingior = PerlACE::LocalFile ("ping.ior");
$pongior = PerlACE::LocalFile ("pong.ior");

unlink $pingior, $pongior;

$PING = new PerlACE::Process ("ping", "-o $pingior");
$PONG = new PerlACE::Process ("pong", "-o $pongior");
$CTRL = new PerlACE::Process ("control", "-f file://$pingior -g file://$pongior");

print STDERR "Starting Ping\n";

$PING->Spawn ();

if (PerlACE::waitforfile_timed ($pingior, 20) == -1) {
    print STDERR "ERROR: cannot find file <$pingior>\n";
    $PING->Kill (); 
    exit 1;
}

print STDERR "Starting Pong\n";

$PONG->Spawn ();
if (PerlACE::waitforfile_timed ($pongior, 20) == -1) {
    print STDERR "ERROR: cannot find file <$pongior>\n";
    $PING->Kill ();
    $PONG->Kill (); 
    exit 1;
}

print STDERR "Starting Control\n";

$control = $CTRL->SpawnWaitKill (200);

if ($control != 0) {
    print STDERR "ERROR: control returned $control\n";
    $status = 1;
}

$pi = $PING->TerminateWaitKill (5);

if ($pi != 0) {
    print STDERR "ERROR: ping returned $pi\n";
    $status = 1;
}

$po = $PONG->TerminateWaitKill (5);

if ($po != 0) {
    print STDERR "ERROR: pong returned $po\n";
    $status = 1;
}

unlink $pingior, $pongior;

exit $status;
