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

my $ifr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $cli = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ti = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$ifriorfile= "if_repo.ior";

my $ifr_ifriorfile = $ifr->LocalFile ($ifriorfile);
my $cli_ifriorfile = $cli->LocalFile ($ifriorfile);
my $ti_ifriorfile = $ti->LocalFile ($ifriorfile);

$ifr->DeleteFile ($ifriorfile);
$cli->DeleteFile ($ifriorfile);
$ti->DeleteFile ($ifriorfile);


$ifr_server = "$ENV{TAO_ROOT}/orbsvcs/IFR_Service/IFR_Service";
$tao_ifr = "$ENV{ACE_ROOT}/bin/tao_ifr";
$test_idl = "test.idl";

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

$IFR = $ifr->CreateProcess ($ifr_server, "-ORBdebuglevel $debug_level ".
                                               "-ORBInitRef InterfaceRepository=file://$ifr_ifriorfile ".
                                               "$lookup_by_name ");
$TI = $ti->CreateProcess ($tao_ifr);
$Cli = $cli->CreateProcess ("IFRBrowser", "-ORBInitRef InterfaceRepository=file://$cli_ifriorfile ".
                                               "$lookup_by_name ");
# We want the tao_ifr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TI->IgnoreExeSubDir (1);

$IFR_status = $IFR->Spawn ();

if ($IFR_status != 0) {
    print STDERR "ERROR: Interface Repository Service returned $IFR_status\n";
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
if ($cli->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($ifriorfile) == -1) {
    print STDERR "ERROR: cannot set file <$ti_ifriorfile>\n";
    $IFR->Kill (); $IFR->TimedWait (1);
    exit 1;
}

$TI->Arguments ("-ORBInitRef InterfaceRepository=file://$ti_ifriorfile $test_idl");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+15);

if ($TI_status != 0) {
    print STDERR "ERROR: tao_ifr (test.idl) returned $Cli_status\n";
    $status = 1;
}

$Cli_status = $Cli->SpawnWaitKill ($cli->ProcessStartWaitInterval());
if ($Cli_status != 0) {
    print STDERR "ERROR: client returned $Cli_status\n";
    $status = 1;
}

# remove the interface from the Interface Repository.
$TI->Arguments ("-ORBInitRef InterfaceRepository=file://$ti_ifriorfile -r $test_idl");

$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval()+15);

if ($TI_status != 0) {
    print STDERR "ERROR: tao_ifr (-r test.idl) returned $TI_status\n";
    $status = 1;
}

$IFR_status = $IFR->TerminateWaitKill ($ifr->ProcessStartWaitInterval());
if ($IFR_status != 0) {
    print STDERR "ERROR: IFR returned $IFR_status\n";
    $status = 1;
}

$ifr->DeleteFile ($ifriorfile);
$cli->DeleteFile ($ifriorfile);
$ti->DeleteFile ($ifriorfile);

exit $status;
