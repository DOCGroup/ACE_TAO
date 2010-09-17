eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::stat;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ping = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $pong = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ctrl = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$pingiorfile = "ping.ior";
$pongiorfile = "pong.ior";

my $ping_pingiorfile = $ping->LocalFile ($pingiorfile);
my $pong_pongiorfile = $pong->LocalFile ($pongiorfile);
my $ctrl_pingiorfile = $ctrl->LocalFile ($pingiorfile);
my $ctrl_pongiorfile = $ctrl->LocalFile ($pongiorfile);
$ping->DeleteFile ($pingiorfile);
$pong->DeleteFile ($pongiorfile);
$ctrl->DeleteFile ($pingiorfile);
$ctrl->DeleteFile ($pongiorfile);

$PING = $ping->CreateProcess ("ping",
                              " -o $ping_pingiorfile");
$PONG = $pong->CreateProcess ("pong",
                              " -o $pong_pongiorfile");
$CTRL = $ctrl->CreateProcess ("control",
                              " -ORBDebugLevel $debug_level ".
                              " -f file://$ctrl_pingiorfile ".
                              " -g file://$ctrl_pongiorfile");

print STDERR "Starting Ping\n";

$PING_status = $PING->Spawn ();

if ($PING_status != 0) {
    print STDERR "ERROR: Ping returned $PING_status\n";
    exit 1;
}

if ($ping->WaitForFileTimed ($pingiorfile,$ping->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ping_pingiorfile>\n";
    $PING->Kill (); $PING->TimedWait (1);
    exit 1;
}

if ($ping->GetFile ($pingiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ping_pingiorfile>\n";
    $PING->Kill (); $PING->TimedWait (1);
    exit 1;
}
if ($ctrl->PutFile ($pingiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ctrl_pingiorfile>\n";
    $PING->Kill (); $PING->TimedWait (1);
    exit 1;
}

print STDERR "Starting Pong\n";

$PONG_status = $PONG->Spawn ();

if ($PONG_status != 0) {
    print STDERR "ERROR: Pong returned $PONG_status\n";
    exit 1;
}

if ($pong->WaitForFileTimed ($pongiorfile,$pong->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$pong_pongiorfile>\n";
    $PONG->Kill (); $PONG->TimedWait (1);
    exit 1;
}

if ($pong->GetFile ($pongiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$pong_pongiorfile>\n";
    $PONG->Kill (); $PONG->TimedWait (1);
    exit 1;
}
if ($ctrl->PutFile ($pongiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ctrl_pongiorfile>\n";
    $PONG->Kill (); $PONG->TimedWait (1);
    exit 1;
}

print STDERR "Starting Control\n";

$CTRL_status = $CTRL->Spawn ();

if ($CTRL_status != 0) {
    print STDERR "ERROR: Control returned $CTRL_status\n";
    exit 1;
}

$PING_status = $PING->WaitKill ($ping->ProcessStartWaitInterval()+85);
if ($PING_status != 0) {
    print STDERR "ERROR: Ping returned $PING_status\n";
    $status = 1;
}

$PONG_status = $PONG->WaitKill ($pong->ProcessStartWaitInterval()+85);
if ($PONG_status != 0) {
    print STDERR "ERROR: Pong returned $PONG_status\n";
    $status = 1;
}

$CTRL_status = $CTRL->TerminateWaitKill ($ctrl->ProcessStopWaitInterval());
if ($CTRL_status != 0) {
    print STDERR "ERROR: Control returned $CTRL_status\n";
    $status = 1;
}

$ping->DeleteFile ($pingiorfile);
$pong->DeleteFile ($pongiorfile);
$ctrl->DeleteFile ($pingiorfile);
$ctrl->DeleteFile ($pongiorfile);

exit $status;
