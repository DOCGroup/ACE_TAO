# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;


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
my $s = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $c = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $iorfile = "ns.ior";

my $ns_iorfile = $ns->LocalFile ($iorfile);
my $s_iorfile = $s->LocalFile ($iorfile);
my $c_iorfile = $c->LocalFile ($iorfile);
$ns->DeleteFile ($iorfile);
$s->DeleteFile ($iorfile);
$c->DeleteFile ($iorfile);

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
$NS = $ns->CreateProcess ($NameService, "-ORBdebuglevel $debug_level ".
                                        " -o $ns_iorfile");
$S = $ns->CreateProcess ("EchoEventSupplier", "-ORBInitRef NameService=file://$s_iorfile");
$C = $ns->CreateProcess ("EchoEventConsumer", "-ORBInitRef NameService=file://$c_iorfile");

# start Naming Service
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($iorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($s->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}


# start EchoEventSupplier
$S_status = $S->Spawn ();

if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
    exit 1;
}

# start EchoEventConsumer
$C_status = $C->Spawn ();

if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    exit 1;
}

$C_status = $C->WaitKill ($c->ProcessStopWaitInterval()+45);

if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    $status = 1;
}

$S_status = $S->TerminateWaitKill ($s->ProcessStopWaitInterval());

if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($iorfile);
$s->DeleteFile ($iorfile);
$c->DeleteFile ($iorfile);

exit $status;

