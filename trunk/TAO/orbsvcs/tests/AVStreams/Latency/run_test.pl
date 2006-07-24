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

$CTRL->Spawn();

$PING->WaitKill(100);

$PONG->WaitKill(100);

$control = $CTRL->TerminateWaitKill (5);

if ($control != 0) {
    print STDERR "ERROR: control returned $control\n";
    $status = 1;
}

unlink $pingior, $pongior;

exit $status;
