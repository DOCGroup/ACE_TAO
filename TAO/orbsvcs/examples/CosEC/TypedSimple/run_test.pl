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
my $ces = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ifr = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ti = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $sup = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $con = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

$nsiorfile = "ns.ior";
$ifriorfile = "ifr.ior";
$cesiorfile = "ec.ior";
$coniorfile = "Consumer.ior";
$idl = "Country.idl";

my $ifr_ifriorfile = $ifr->LocalFile ($ifriorfile);
my $ti_ifriorfile = $ti->LocalFile ($ifriorfile);
my $ces_ifriorfile = $ces->LocalFile ($ifriorfile);
my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $ces_nsiorfile = $ces->LocalFile ($nsiorfile);
my $sup_nsiorfile = $sup->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $ces_cesiorfile = $ces->LocalFile ($cesiorfile);
my $con_coniorfile = $con->LocalFile ($coniorfile);
my $ti_idl = $ti->LocalFile ($idl);

$ifr->DeleteFile ($ifrorfile);
$ti->DeleteFile ($ifriorfile);
$ces->DeleteFile ($ifriorfile);
$ns->DeleteFile ($nsiorfile);
$ces->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$ces->DeleteFile ($cesiorfile);
$con->DeleteFile ($coniorfile);


$IFR = $ifr->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/IFR_Service/IFR_Service",
                            " -o $ifr_ifriorfile");
$TI = $ti->CreateProcess ($ENV{"ACE_ROOT"}."/bin/tao_ifr",
                          " -ORBInitRef InterfaceRepository=file://$ti_ifriorfile ".
                          "-I".$ENV{"TAO_ROOT"}."/orbsvcs $ti_idl");
$NS = $ns->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/Naming_Service/Naming_Service",
                          "-o $ns_nsiorfile");
$CES = $ces->CreateProcess ($ENV{"TAO_ROOT"}."/orbsvcs/CosEvent_Service/CosEvent_Service",
                          "-n CountryEventChannel -r -t -d -o $ces_cesiorfile ".
                          "-ORBInitRef InterfaceRepository=file://$ces_ifriorfile ".
                          "-ORBInitRef NameService=file://$ces_nsiorfile ");
$CON = $con->CreateProcess ("Consumer",
                          "-o $con_coniorfile ".
                          "-ORBInitRef NameService=file://$con_nsiorfile ");
$SUP = $sup->CreateProcess ("Supplier",
                          "-ORBInitRef NameService=file://$sup_nsiorfile ");

$IFR_status = $IFR->Spawn ();

if ($IFR_status != 0) {
    print STDERR "ERROR: IFR_Service returned $IFR_status\n";
    exit 1;
}

if ($ifr->WaitForFileTimed ($ifriorfile,$ifr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ifr_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

if ($ifr->GetFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ifr_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}
if ($ces->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ces_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}


$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+45);

if ($TI_status != 0) {
    print STDERR "ERROR: tao_idl returned $TI_status\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

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
if ($ces->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ces_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

$CES_status = $CES->Spawn ();

if ($CES_status != 0) {
    print STDERR "ERROR: CosEvent Service returned $CES_status\n";
    exit 1;
}

if ($ces->WaitForFileTimed ($cesiorfile,$ces->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ces_cesiorfile>\n";
    $CES->Kill (); $CES->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

$CON_status = $CON->Spawn ();

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    exit 1;
}

if ($con->WaitForFileTimed ($coniorfile,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_coniorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $CES->Kill (); $CES->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+105);

if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->TerminateWaitKill ($con->ProcessStopWaitInterval());

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status = 1;
}

$CES_status = $CES->TerminateWaitKill ($ces->ProcessStopWaitInterval());

if ($CES_status != 0) {
    print STDERR "ERROR: CosEvent Service returned $CES_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$IFR_status = $IFR->TerminateWaitKill ($ifr->ProcessStopWaitInterval());

if ($IFR_status != 0) {
    print STDERR "ERROR: IFR_Service returned $IFR_status\n";
    $status = 1;
}

$ifr->DeleteFile ($ifrorfile);
$ti->DeleteFile ($ifriorfile);
$ces->DeleteFile ($ifriorfile);
$ns->DeleteFile ($nsiorfile);
$ces->DeleteFile ($nsiorfile);
$sup->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$ces->DeleteFile ($cesiorfile);
$con->DeleteFile ($coniorfile);

exit $status;
