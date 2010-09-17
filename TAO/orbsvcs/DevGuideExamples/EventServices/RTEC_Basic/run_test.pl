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
my $es = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $s = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $c = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $nsiorfile = "ns.ior";
my $esiorfile = "es.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $es_nsiorfile = $es->LocalFile ($nsiorfile);
my $s_nsiorfile = $s->LocalFile ($nsiorfile);
my $c_nsiorfile = $c->LocalFile ($nsiorfile);
my $es_esiorfile = $es->LocalFile ($esiorfile);
$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$es->DeleteFile ($esiorfile);

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = $ns->CreateProcess ($NameService, "-ORBdebuglevel $debug_level ".
                                        " -o $ns_nsiorfile");
$EventService = "$ENV{TAO_ROOT}/orbsvcs/Event_Service/Event_Service";
$ES = $es->CreateProcess ($EventService, " -o $es_esiorfile ".
                                        "-ORBInitRef NameService=file://$es_nsiorfile");
$S = $ns->CreateProcess ("EchoEventSupplier", "-ORBInitRef NameService=file://$s_nsiorfile");
$C = $ns->CreateProcess ("EchoEventConsumer", "-ORBInitRef NameService=file://$c_nsiorfile");

# start Naming Service
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
if ($es->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$es_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($s->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}


# start Event Service
$ES_status = $ES->Spawn ();

if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    exit 1;
}

if ($es->WaitForFileTimed ($esiorfile,$es->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$es_esiorfile>\n";
    $ES->Kill (); $ES->TimedWait (1);
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

$ES_status = $ES->TerminateWaitKill ($es->ProcessStopWaitInterval());

if ($ES_status != 0) {
    print STDERR "ERROR: Event Service returned $ES_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);
$c->DeleteFile ($nsiorfile);
$es->DeleteFile ($esiorfile);

exit $status;
