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
my $t = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $c = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $s = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$nsiorfile = "ns.ior";
$conffile = "ec" . "$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $t_nsiorfile = $t->LocalFile ($nsiorfile);
my $c_nsiorfile = $c->LocalFile ($nsiorfile);
my $s_nsiorfile = $s->LocalFile ($nsiorfile);
my $t_conffile = $t->LocalFile ($conffile);
$ns->DeleteFile ($nsiorfile);
$t->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");

$T = $t->CreateProcess ("Service",
                          " -ORBInitRef NameService=file://$t_nsiorfile ".
                          "-ORBsvcconf $t_conffile ");

$C = $c->CreateProcess ("Consumer",
                        " -ORBInitRef NameService=file://$c_nsiorfile ");

$S = $s->CreateProcess ("Supplier",
                        " -ORBInitRef NameService=file://$s_nsiorfile ");


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
if ($t->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$t_nsiorfile>\n";
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

print STDOUT "Starting event server\n";
$T_status = $T->Spawn ();

if ($T_status != 0) {
    print STDERR "ERROR: Service returned $T_status\n";
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
$S_status = $S->SpawnWaitKill ($s->ProcessStartWaitInterval()+105);
if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
    exit 1;
}

$C_status = $C->WaitKill ($c->ProcessStopWaitInterval());
if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    $status = 1;
}

$T_status = $T->TerminateWaitKill ($t->ProcessStopWaitInterval());

if ($T_status != 0) {
    print STDERR "ERROR: service returned $T_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$t->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);

exit $status;
