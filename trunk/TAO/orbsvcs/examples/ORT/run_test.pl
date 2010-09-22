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

my $ifr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ti = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $gws = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $srv = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $cl2 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

$ifriorfile= "if_repo.ior";
$srviorfile = "iorfile.ior";
$gwsiorfile = "gateway_ior.ior";
$testidl = "sum_server.idl";

my $ifr_ifriorfile = $ifr->LocalFile ($ifriorfile);
my $gws_ifriorfile = $gws->LocalFile ($ifriorfile);
my $ti_ifriorfile = $ti->LocalFile ($ifriorfile);
my $gws_gwsiorfile = $gws->LocalFile ($gwsiorfile);
my $srv_gwsiorfile = $srv->LocalFile ($gwsiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cl2_srviorfile = $cl2->LocalFile ($srviorfile);
my $ti_testidl = $ti->LocalFile ($testidl);
$ifr->DeleteFile ($ifriorfile);
$gws->DeleteFile ($ifriorfile);
$ti->DeleteFile ($ifriorfile);
$gws->DeleteFile ($gwsiorfile);
$srv->DeleteFile ($gwsiorfile);
$srv->DeleteFile ($srviorfile);
$cl2->DeleteFile ($srviorfile);

# find the tao_ifr executable.
# Its placement is dependent upon the OS and if MPC generated makefiles are used.
my $exec_extn="";
if ($^O eq "MSWin32") {
    $exec_extn=".exe";
}

$tao_ifr = "../../../../bin/tao_ifr";
if (! -e $tao_ifr . $exec_extn ) {
    $tao_ifr = "../../../IFR_Service/tao_ifr";
    if (! -e $tao_ifr . $exec_extn ) {
        print STDERR "ERROR: tao_ifr compiler not found.\n";
        exit 1;
    }
}

$lookup_by_name = "";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-n") {
        $lookup_by_name = "-n";
    }
    else {
        $other .= $ARGV[$i];
    }
}

$TI = $ti->CreateProcess ($tao_ifr);
$IFR = $ifr->CreateProcess ("../../IFR_Service/tao_ifr_service",
                            "-o $ifr_ifriorfile");
$GWS = $gws->CreateProcess ("gateway_server",
                            "-o $gws_gwsiorfile ".
                            "-ORBInitRef IFR_Service=file://$gws_ifriorfile");
$SRV = $srv->CreateProcess ("server",
                            "-o $srv_srviorfile ".
                            "-ORBInitRef Gateway_Object_Factory=file://$gws_gwsiorfile");
$CL2 = $cl2->CreateProcess ("client",
                            "-k file://$cl2_srviorfile");

$IFR_status = $IFR->Spawn ();

if ($IFR_status != 0) {
    print STDERR "ERROR: IFR Service returned $IFR_status\n";
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
if ($gws->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$gws_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

$TI->Arguments ("-ORBInitRef InterfaceRepository=file://$ti_ifriorfile $ti_testidl");
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+15);

$GWS_status = $GWS->Spawn ();

if ($GWS_status != 0) {
    print STDERR "ERROR: Gateway returned $GWS_status\n";
    exit 1;
}

if ($gws->WaitForFileTimed ($gwsiorfile,$gws->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$gws_gwsiorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    exit 1;
}

if ($gws->GetFile ($gwsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$gws_gwsiorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    exit 1;
}
if ($srv->PutFile ($gwsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_gwsiorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    exit 1;
}

$SRV_status = $SRV->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Server returned $SRV_status\n";
    exit 1;
}

if ($srv->WaitForFileTimed ($srviorfile,$srv->ProcessStartWaitInterval()+1485) == -1) {
    print STDERR "ERROR: cannot find file <$srv_srviorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}
if ($cl2->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl2_srviorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    $GWS->Kill (); $GWS->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    exit 1;
}

if ($TI_status != 0) {
    print STDERR "ERROR: tao_idl returned $TI_status\n";
    $status = 1;
}

$CL2_status = $CL2->SpawnWaitKill ($cl2->ProcessStartWaitInterval()+45);
if ($CL2_status != 0) {
    print STDERR "ERROR: Client returned $CL2_status\n";
    $status = 1;
}

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+15);
if ($TI_status != 0) {
    print STDERR "ERROR: tao_idl returned $TI_status\n";
    $status = 1;
}

$GWS_status = $GWS->TerminateWaitKill ($gws->ProcessStopWaitInterval());

if ($GWS_status != 0) {
    print STDERR "ERROR: Gateway returned $GWS_status\n";
    $status = 1;
}

$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Server returned $SRV_status\n";
    $status = 1;
}

$IFR_status = $IFR->TerminateWaitKill ($ifr->ProcessStopWaitInterval());

if ($IFR_status != 0) {
    print STDERR "ERROR: IFR Service returned $IFR_status\n";
    $status = 1;
}

$ifr->DeleteFile ($ifriorfile);
$gws->DeleteFile ($ifriorfile);
$ti->DeleteFile ($ifriorfile);
$gws->DeleteFile ($gwsiorfile);
$srv->DeleteFile ($gwsiorfile);
$srv->DeleteFile ($srviorfile);
$cl2->DeleteFile ($srviorfile);

exit $status;
