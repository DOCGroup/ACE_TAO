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

my $c1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $c2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $imr = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $act = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $ti = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $si = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $sdn = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";
my $imr_activator = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_activator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

# The Tests
$implrepo_ior = "implrepo.ior";
$activator_ior = "activator.ior";
$messenger_ior = "Messenger.ior";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $act_imriorfile = $act->LocalFile ($implrepo_ior);
my $ti_imriorfile = $ti->LocalFile ($implrepo_ior);
my $si_imriorfile = $si->LocalFile ($implrepo_ior);
my $sdn_imriorfile = $sdn->LocalFile ($implrepo_ior);
my $act_actiorfile = $act->LocalFile ($activator_ior);
my $imr_srviorfile = $imr->LocalFile ($messenger_ior);
my $c1_srviorfile = $c1->LocalFile ($messenger_ior);
my $c2_srviorfile = $c2->LocalFile ($messenger_ior);
my $si_srviorfile = $si->LocalFile ($messenger_ior);

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($implrepo_ior);
$act->DeleteFile ($implrepo_ior);
$ti->DeleteFile ($implrepo_ior);
$si->DeleteFile ($implrepo_ior);
$sdn->DeleteFile ($implrepo_ior);
$act->DeleteFile ($activator_ior);
$imr->DeleteFile ($messenger_ior);
$c1->DeleteFile ($messenger_ior);
$c2->DeleteFile ($messenger_ior);
$si->DeleteFile ($messenger_ior);

# Note : We don't actually use SVR, but we need a way to get the
#        path to the -ExeSubDir
$SVR = $imr->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level");
my $server = $SVR->Executable ();
my $srv_server = $imr->LocalFile ($server);

$IR = $imr->CreateProcess ($implrepo_server, "-d 1 ".
                                               "orbobjrefstyle url ".
                                               "-t 5 ".
                                               "-o $imr_imriorfile");
print ">>> " . $IR->CommandLine() . "\n";

$ACT = $act->CreateProcess ($imr_activator, "-d 1 ".
                                               "orbobjrefstyle url ".
                                               "-o $act_actiorfile ".
                                               "-ORBInitRef ImplRepoService=file://$act_imriorfile");

$TI = $ti->CreateProcess ($tao_imr, "-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                                               "add MessengerService ".
                                               "-c \"$srv_server -orbobjrefstyle url -ORBUseIMR 1 -ORBInitRef ImplRepoService=file://$imr_imriorfile\" ");



$SI = $si->CreateProcess ($tao_imr, "-ORBInitRef ImplRepoService=file://$si_imriorfile ".
                                               "ior MessengerService ".
                                               "-f $si_srviorfile ");

$C1 = $c1->CreateProcess ("MessengerClient", "-k file://$c1_srviorfile ");

$SDN = $sdn->CreateProcess ("$tao_imr", "-ORBInitRef ImplRepoService=file://$sdn_imriorfile ".
                                        "shutdown MessengerService");

$C2 = $c2->CreateProcess ("MessengerClient", "-k file://$c2_srviorfile ");


$IR_status = $IR->Spawn ();

if ($IR_status != 0) {
    print STDERR "ERROR: ImplRepo Service returned $IR_status\n";
    exit 1;
}

if ($imr->WaitForFileTimed ($implrepo_ior,$imr->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$imr_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($act->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$act_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($si->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$si_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}
if ($sdn->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$sdn_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

print ">>> " . $ACT->CommandLine() . "\n";
$ACT_status = $ACT->Spawn ();

if ($ACT_status != 0) {
    print STDERR "ERROR: ImR_Activator returned $ACT_status\n";
    exit 1;
}

if ($act->WaitForFileTimed ($activator_ior,$act->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$act_actiorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

print ">>> " . $TI->CommandLine() . "\n";
# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.

$TI->IgnoreExeSubDir (1);

## Note : Instead of using tao_imr to generate the ior, it's easy enough
## to just create one by hand. The ior is just a normal corbaloc ior with
## the poa_name of the server and ip address of the imr.
## (ie corbaloc::localhost:8888/Messengerservice)
## Of course, to do this, you'd have to start the imr on port 8888.
## We use the "tao_imr ior" command, because we don't know which port was used.
$TI_status = $TI->SpawnWaitKill ($ti->ProcessStartWaitInterval());

if ($TI_status != 0) {
    print STDERR "ERROR: tao_imr returned $TI_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

print ">>> " . $SI->CommandLine() . "\n";
$SI->IgnoreExeSubDir (1);


$SI_status = $SI->SpawnWaitKill ($si->ProcessStartWaitInterval());

if ($SI_status != 0) {
    print STDERR "ERROR: tao_imr returned $SI_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

if ($si->WaitForFileTimed ($messenger_ior,$si->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$si_srviorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

if ($si->GetFile ($messenger_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$si_srviorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($messenger_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_srviorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}
if ($c2->PutFile ($messenger_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c2_srviorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}


print ">>> " . $C1->CommandLine() . "\n";
$C1_status = $C1->SpawnWaitKill ($c1->ProcessStartWaitInterval());

if ($C1_status != 0) {
    print STDERR "ERROR: Client1 returned $C1_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

$SDN->IgnoreExeSubDir (1);
$SDN_status = $SDN->SpawnWaitKill ($sdn->ProcessStartWaitInterval());

if ($SDN_status != 0) {
    print STDERR "ERROR: Shutdown returned $SDN_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

$C2_status = $C2->SpawnWaitKill ($c2->ProcessStartWaitInterval());

if ($C2_status != 0) {
    print STDERR "ERROR: Client2 returned $C2_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}


$SDN_status = $SDN->SpawnWaitKill ($sdn->ProcessStartWaitInterval());

if ($SDN_status != 0) {
    print STDERR "ERROR: Shutdown returned $SDN_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    $ACT->Kill (); $ACT->TimedWait (1);
    exit 1;
}

$ACT_status = $ACT->TerminateWaitKill ($act->ProcessStopWaitInterval());

if ($ACT_status != 0) {
    print STDERR "ERROR: ImR_Activator returned $ACT_status\n";
    $status = 1;
}

$IR_status = $IR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($IR_status != 0) {
    print STDERR "ERROR: ImplRepo Server returned $IR_status\n";
    $status = 1;
}


$imr->DeleteFile ($implrepo_ior);
$act->DeleteFile ($implrepo_ior);
$ti->DeleteFile ($implrepo_ior);
$si->DeleteFile ($implrepo_ior);
$sdn->DeleteFile ($implrepo_ior);
$act->DeleteFile ($activator_ior);
$imr->DeleteFile ($messenger_ior);
$c1->DeleteFile ($messenger_ior);
$c2->DeleteFile ($messenger_ior);
$si->DeleteFile ($messenger_ior);

exit $status;
