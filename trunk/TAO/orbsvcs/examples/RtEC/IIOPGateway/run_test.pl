eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $t1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $t2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $g = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $c = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $s = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

$nsiorfile = "ns.ior";
$conffile = "ec" . "$PerlACE::svcconf_ext";
$gconffile = "gateway" . "$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $t1_nsiorfile = $t1->LocalFile ($nsiorfile);
my $t2_nsiorfile = $t2->LocalFile ($nsiorfile);
my $g_nsiorfile = $g->LocalFile ($nsiorfile);
my $c_nsiorfile = $c->LocalFile ($nsiorfile);
my $s_nsiorfile = $s->LocalFile ($nsiorfile);
my $t1_conffile = $t1->LocalFile ($conffile);
my $t2_conffile = $t2->LocalFile ($conffile);
my $g_gconffile = $g->LocalFile ($gconffile);
$ns->DeleteFile ($nsiorfile);
$t1->DeleteFile ($nsiorfile);
$t2->DeleteFile ($nsiorfile);
$g->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");

$T1 = $t1->CreateProcess ("EC",
                          " -ORBInitRef NameService=file://$t1_nsiorfile ".
                          "-ORBsvcconf $t1_conffile ".
                          "-e channel1");

$T2 = $t2->CreateProcess ("EC",
                          " -ORBInitRef NameService=file://$t2_nsiorfile ".
                          "-ORBsvcconf $t2_conffile ".
                          "-e channel2");

$G = $g->CreateProcess ("Gateway",
                        " -ORBInitRef NameService=file://$g_nsiorfile ".
                        "-ORBSvcconf $g_gconffile ".
                        "-c channel2 ".
                        "-s channel1");

$C = $c->CreateProcess ("Consumer",
                        " -ORBInitRef NameService=file://$c_nsiorfile ".
                        "-ORBDebugLevel $debug_level ".
                        "-e channel2");

$S = $s->CreateProcess ("Supplier",
                        " -ORBInitRef NameService=file://$s_nsiorfile ".
                        "-e channel1");

print STDOUT "Starting name server\n";
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($t1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$t1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($t2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$t2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($g->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$g_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($s->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDOUT "Starting event channel 1\n";
$T1_status = $T1->Spawn ();

if ($T1_status != 0) {
    print STDERR "ERROR: T1 returned $T1_status\n";
    exit 1;
}

sleep 2;

print STDOUT "Starting event channel 2\n";
$T2_status = $T2->Spawn ();

if ($T2_status != 0) {
    print STDERR "ERROR: T2 returned $T2_status\n";
    exit 1;
}

sleep 2;

print STDOUT "Starting gateway\n";
$G_status = $G->Spawn ();

if ($G_status != 0) {
    print STDERR "ERROR: Gateway returned $G_status\n";
    exit 1;
}

sleep 2;

print STDOUT "Starting consumer\n";
$C_status = $C->Spawn ();

if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    exit 1;
}

sleep 1;

print STDOUT "Starting supplier\n";
$S_status = $S->Spawn ();

if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
    exit 1;
}

sleep 1;

$C_status = $C->WaitKill ($c->ProcessStopWaitInterval());
if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    $status = 1;
}

$T2_status = $T2->TerminateWaitKill ($t2->ProcessStopWaitInterval());

if ($T2_status != 0) {
    print STDERR "ERROR: service returned $T2_status\n";
    $status = 1;
}

print STDOUT "15 seconds before termination...\n";
sleep 15;

$S_status = $S->WaitKill ($s->ProcessStopWaitInterval());
if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
    $status = 1;
}

$T1_status = $T1->TerminateWaitKill ($t1->ProcessStopWaitInterval());

if ($T1_status != 0) {
    print STDERR "ERROR: service returned $T1_status\n";
    $status = 1;
}

$G_status = $G->TerminateWaitKill ($g->ProcessStopWaitInterval());

if ($G_status != 0) {
    print STDERR "ERROR: Gateway returned $G_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$t1->DeleteFile ($nsiorfile);
$t2->DeleteFile ($nsiorfile);
$g->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);

exit $status;
