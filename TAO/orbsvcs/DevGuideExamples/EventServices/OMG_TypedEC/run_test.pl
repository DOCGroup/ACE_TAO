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

my $ifr= PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ti = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ns = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $es = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $c = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $s = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

my $nsiorfile = "ns.ior";
my $ifriorfile = "ifr.ior";
my $esiorfile = "es.ior";
my $consiorfile = "Consumer.ior";
my $idlfile = "Messenger.idl";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $es_esiorfile = $es->LocalFile ($esiorfile);
my $ifr_ifriorfile = $ifr->LocalFile ($ifriorfile);
my $c_consiorfile = $c->LocalFile ($consiorfile);
my $ti_ifriorfile = $ti->LocalFile ($ifriorfile);
my $es_nsiorfile = $es->LocalFile ($nsiorfile);
my $es_ifriorfile = $es->LocalFile ($ifriorfile);
my $c_nsiorfile = $c->LocalFile ($nsiorfile);
my $s_nsiorfile = $s->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($esiorfile);
$ifr->DeleteFile ($ifriorfile);
$c->DeleteFile ($consiorfile);
$ti->DeleteFile ($ifriorfile);
$es->DeleteFile ($nsiorfile);
$es->DeleteFile ($ifriorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);


$IFRService = "$ENV{TAO_ROOT}/orbsvcs/IFR_Service/tao_ifr_service";
$IF = $ifr->CreateProcess ($IFRService,  "-ORBdebuglevel $debug_level ".
                                         "-o $ifr_ifriorfile");
$TAO_IFR = "$ENV{ACE_ROOT}/bin/tao_ifr";
$TI = $ti->CreateProcess ($TAO_IFR, "-ORBInitRef InterfaceRepository=file://$ti_ifriorfile ".
                                    "$idlfile");
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
$NS = $ns->CreateProcess ($NameService, " -o $ns_nsiorfile");
$EventService = "$ENV{TAO_ROOT}/orbsvcs/Event_Service/tao_cosevent";
$ES = $es->CreateProcess ($EventService, " -t -o $es_esiorfile ".
                                        "-ORBInitRef NameService=file://$es_nsiorfile ".
                                        "-ORBInitRef InterfaceRepository=file://$es_ifriorfile ");
$C = $c->CreateProcess ("Consumer", "-ORBInitRef NameService=file://$c_nsiorfile");
$S = $s->CreateProcess ("Supplier", "-ORBInitRef NameService=file://$s_nsiorfile");


# start the Interface Repository Service
$IF_status = $IF->Spawn ();

if ($IF_status != 0) {
    print STDERR "ERROR: Interface Repository returned $IF_status\n";
    exit 1;
}

if ($ifr->WaitForFileTimed ($ifriorfile,$ifr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_ifriorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}

if ($ifr->GetFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ifr_ifriorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_ifriorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}
if ($es->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$es_ifriorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}

# load the IFR with the Messenger interface info
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);

if ($TI_status != 0) {
    print STDERR "ERROR: tao_ifr returned $TI_status\n";
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}

# start Naming Service
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($es->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$es_nsiorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($c->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$c_nsiorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($s->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$s_nsiorfile>\n";
    $IF->Kill (); $IF->TimedWait (1);
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
    $NS->Kill (); $NS->TimedWait (1);
    $IF->Kill (); $IF->TimedWait (1);
    exit 1;
}

# start Consumer
$C_status = $C->Spawn ();

if ($C_status != 0) {
    print STDERR "ERROR: Consumer returned $C_status\n";
    exit 1;
}

# start Supplier
$S_status = $S->Spawn ();

if ($S_status != 0) {
    print STDERR "ERROR: Supplier returned $S_status\n";
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

$IF_status = $IF->TerminateWaitKill ($ifr->ProcessStopWaitInterval());

if ($IF_status != 0) {
    print STDERR "ERROR: Interface Repository returned $IF_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$es->DeleteFile ($esiorfile);
$ifr->DeleteFile ($ifriorfile);
$c->DeleteFile ($consiorfile);
$ti->DeleteFile ($ifriorfile);
$es->DeleteFile ($nsiorfile);
$es->DeleteFile ($ifriorfile);
$c->DeleteFile ($nsiorfile);
$s->DeleteFile ($nsiorfile);

exit $status;
