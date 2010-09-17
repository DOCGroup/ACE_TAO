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

my $lm = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ns = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sv = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $sr = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $cl = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $cr = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

$srviorfile = "obj.ior";
$lmiorfile = "lm.ior";
$nsiorfile = "ns.ior";

my $lm_lmiorfile = $lm->LocalFile ($lmiorfile);
my $sv_lmiorfile = $sv->LocalFile ($lmiorfile);
my $sr_lmiorfile = $sr->LocalFile ($lmiorfile);
my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv_nsiorfile = $sv->LocalFile ($nsiorfile);
my $sr_nsiorfile = $sr->LocalFile ($nsiorfile);
my $cl_nsiorfile = $cl->LocalFile ($nsiorfile);
my $cr_nsiorfile = $cr->LocalFile ($nsiorfile);
my $sv_srviorfile = $sv->LocalFile ($srviorfile);
my $cl_srviorfile = $cl->LocalFile ($srviorfile);
my $sr_srviorfile = $sr->LocalFile ($srviorfile);
my $cr_srviorfile = $cr->LocalFile ($srviorfile);
$lm->DeleteFile ($lmiorfile);
$sv->DeleteFile ($lmiorfile);
$sr->DeleteFile ($lmiorfile);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sr->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cr->DeleteFile ($nsiorfile);
$sv->DeleteFile ($srviorfile);
$cl->DeleteFile ($srviorfile);
$sr->DeleteFile ($srviorfile);
$cr->DeleteFile ($srviorfile);

$LM = $lm->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/LoadBalancer/LoadManager",
                          " -o $lm_lmiorfile");
$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");
$SV = $sv->CreateProcess ("server",
                          "-o $sv_srviorfile ".
                          " -ORBInitRef LoadManager=file://$sv_lmiorfile ".
                          " -n 1 ".
                          "-ORBInitRef NameService=file://$sv_nsiorfile ".
                          " -s LeastLoaded ".
                          " -r 9000 -c 10300 -d 0.1 ");
$SR = $sr->CreateProcess ("server",
                          "-o $sr_srviorfile ".
                          "-ORBInitRef LoadManager=file://$sr_lmiorfile ".
                          " -n 2 ".
                          "-ORBInitRef NameService=file://sr_nsiorfile ".
                          " -s LeastLoaded ".
                          "-r 9000 -c 10300 -d 0.1 ");
$CL = $cl->CreateProcess ("client",
                          "-k file://$cl_srviorfile ".
                          " -i 5 -n 1 ".
                          "-ORBInitRef NameService=file://cl_nsiorfile ");
$CR = $cr->CreateProcess ("client",
                          "-k file://$cr_srviorfile ".
                          "-i 5 -n 2 ".
                          "-ORBInitRef NameService=file://cr_nsiorfile ");

$LM_status = $LM->Spawn ();

if ($LM_status != 0) {
    print STDERR "ERROR: Load Manager returned $LM_status\n";
    exit 1;
}

if ($lm->WaitForFileTimed ($lmiorfile,$lm->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$lm_lmiorfile>\n";
    $LM->Kill (); $LM->TimedWait (1);
    exit 1;
}

if ($lm->GetFile ($lmiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$lm_lmiorfile>\n";
    $LM->Kill (); $LM->TimedWait (1);
    exit 1;
}
if ($sv->PutFile ($lmiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_lmiorfile>\n";
    $LM->Kill (); $LM->TimedWait (1);
    exit 1;
}
if ($sr->PutFile ($lmiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sr_lmiorfile>\n";
    $LM->Kill (); $LM->TimedWait (1);
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
if ($sv->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sr->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sr_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cl->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cr->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cr_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}


$SV_status = $SV->Spawn ();

if ($SV_status != 0) {
    print STDERR "ERROR: Server returned $SV_status\n";
    exit 1;
}

if ($sv->WaitForFileTimed ($srviorfile,$sv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sv_srviorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($sv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sv_srviorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($cl->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_srviorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
$sv->DeleteFile ($srviorfile);

$SR_status = $SR->Spawn ();

if ($SR_status != 0) {
    print STDERR "ERROR: Server returned $SR_status\n";
    exit 1;
}

if ($sr->WaitForFileTimed ($srviorfile,$sr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sr_srviorfile>\n";
    $SR->Kill (); $SR->TimedWait (1);
    exit 1;
}

if ($sr->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sr_srviorfile>\n";
    $SR->Kill (); $SR->TimedWait (1);
    exit 1;
}
if ($cr->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cr_srviorfile>\n";
    $SR->Kill (); $SR->TimedWait (1);
    exit 1;
}
$sr->DeleteFile ($srviorfile);


$CL_status = $CL->SpawnWaitKill ($cl->ProcessStartWaitInterval()+85);
if ($CL_status != 0) {
    print STDERR "ERROR: client returned $CL_status\n";
    $status = 1;
}

$CR_status = $CR->SpawnWaitKill ($cr->ProcessStartWaitInterval()+85);
if ($CR_status != 0) {
    print STDERR "ERROR: client returned $CR_status\n";
    $status = 1;
}

$SV_status = $SV->WaitKill ($sv->ProcessStartWaitInterval());
if ($SV_status != 0) {
    print STDERR "ERROR: server returned $SV_status\n";
    $status = 1;
}

$LM_status = $LM->TerminateWaitKill ($lm->ProcessStopWaitInterval());

if ($LM_status != 0) {
    print STDERR "ERROR: LoadManager returned $LM_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$lm->DeleteFile ($lmiorfile);
$sv->DeleteFile ($lmiorfile);
$sr->DeleteFile ($lmiorfile);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sr->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cr->DeleteFile ($nsiorfile);
$sv->DeleteFile ($srviorfile);
$cl->DeleteFile ($srviorfile);
$sr->DeleteFile ($srviorfile);
$cr->DeleteFile ($srviorfile);

exit $status;
