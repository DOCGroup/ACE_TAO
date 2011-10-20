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

my $srv = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $cli = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $imr = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $WAIT_TIMEOUT = 2;
my $DEBUG_LEVEL = 1;
my $OBJ_REF_STYLE = "-orbobjrefstyle url";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";

my $implrepo_ior = "imr.ior";
my $messenger_ior = "messenger.ior";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $srv_imriorfile = $srv->LocalFile ($implrepo_ior);
my $srv_srviorfile = $srv->LocalFile ($messenger_ior);
my $cli_srviorfile = $cli->LocalFile ($messenger_ior);

$Srv = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-orbuseimr 1 ".
                                               "$OBJ_REF_STYLE ".
                                               "-ORBInitRef ImplRepoService=file://$srv_imriorfile");
$Cli = $cli->CreateProcess ("MessengerClient", "-k file://$cli_srviorfile");

sub CleanupOutput {
    $imr->DeleteFile ($implrepo_ior);
    $srv->DeleteFile ($implrepo_ior);
    $srv->DeleteFile ($messenger_ior);
    $cli->DeleteFile ($messenger_ior);
}

# Use url object reference style for readability, and startup timeout of 2 seconds.
# Unlike the chapter we'll forgo using -m, because we want to be able to run this
# as a test in our nightly builds, and multicast could interfere with other machines.
$ImR = $imr->CreateProcess ($implrepo_server, "-d $DEBUG_LEVEL ".
                                               "$OBJ_REF_STYLE ".
                                               "-t 2 ".
                                               "-o $imr_imriorfile");

CleanupOutput();

#### Start the example
print ">>> " . $ImR->CommandLine() . "\n";
$ImR_status = $ImR->Spawn ();

if ($ImR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $ImR_status\n";
    exit 1;
}

if ($imr->WaitForFileTimed ($implrepo_ior,$imr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($srv->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$srv_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

print ">>> " . $Srv->CommandLine() . "\n";
$Srv_status = $Srv->Spawn ();

if ($Srv_status != 0) {
    print STDERR "ERROR: Server returned $Srv_status\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

if ($srv->WaitForFileTimed ($messenger_ior,$srv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv_srviorfile>\n";
    $Srv->Kill (); $Srv->TimedWait (1);
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($messenger_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $Srv->Kill (); $Srv->TimedWait (1);
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($messenger_ior) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    $Srv->Kill (); $Srv->TimedWait (1);
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

$Cli_status = $Cli->SpawnWaitKill ($cli->ProcessStartWaitInterval());
if ($Cli_status != 0) {
    print STDERR "ERROR: Client failed to run correctly. Client returned $Cli_status\n";
    $status = 1;
}

$Srv_status = $Srv->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($Srv_status != 0) {
    print STDERR "ERROR: Server returned $Srv_status\n";
    $status = 1;
}

$ImR_status = $ImR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($ImR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $ImR_status\n";
    $status = 1;
}

#### Clean up any output files

CleanupOutput();

exit $status;
