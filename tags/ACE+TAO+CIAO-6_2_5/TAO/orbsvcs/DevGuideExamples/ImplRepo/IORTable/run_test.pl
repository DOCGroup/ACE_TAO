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
my $imr = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ti = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $WAIT_TIMEOUT = 2;
my $DEBUG_LEVEL = 1;
my $OBJ_REF_STYLE = "-orbobjrefstyle url";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";
my $tao_imr = "$ENV{ACE_ROOT}/bin/tao_imr";

my $implrepo_ior = "imr.ior";
my $messenger1_ior = "messenger1.ior";
my $messenger2_ior = "messenger2.ior";
my $messenger3_ior = "messenger3.ior";
my $messenger4_ior = "messenger4.ior";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $ti_imriorfile = $ti->LocalFile ($implrepo_ior);
my $imr_m1iorfile = $imr->LocalFile ($messenger1_ior);
my $c1_m1iorfile = $c1->LocalFile ($messenger1_ior);
my $imr_m2iorfile = $imr->LocalFile ($messenger2_ior);
my $c1_m2iorfile = $c1->LocalFile ($messenger2_ior);
my $c1_m3iorfile = $c1->LocalFile ($messenger3_ior);
my $ti_m3iorfile = $ti->LocalFile ($messenger3_ior);
my $c1_m4iorfile = $c1->LocalFile ($messenger4_ior);
my $ti_m4iorfile = $ti->LocalFile ($messenger4_ior);

sub CleanupOutput {
    $imr->DeleteFile ($implrepo_ior);
    $ti->DeleteFile ($implrepo_ior);
    $imr->DeleteFile ($messenger1_ior);
    $c1->DeleteFile ($messenger1_ior);
    $imr->DeleteFile ($messenger2_ior);
    $c1->DeleteFile ($messenger2_ior);
    $c1->DeleteFile ($messenger3_ior);
    $ti->DeleteFile ($messenger3_ior);
    $c1->DeleteFile ($messenger4_ior);
    $ti->DeleteFile ($messenger4_ior);
}

sub SpawnWait {
    my $process = shift;
    my $file = shift;
    my $target = shift;

    print ">>> " . $process->CommandLine () . "\n";
    my $status = $process->Spawn ();
    if ($status != 0) {
        print STDERR "ERROR: Process returned $status\n";
        return 1;
    }
    my $ret = $target->WaitForFileTimed ($file,$target->ProcessStartWaitInterval());
    if ($ret == -1) {
         print STDERR "ERROR: Cannot find file <$file>\n";
    }
    return $ret;
}

sub SpawnWaitKill {
    my $process = shift;
    my $target = shift;

    print ">>> " . $process->CommandLine() . "\n";
    return $process->SpawnWaitKill($target->ProcessStartWaitInterval());
}

sub RunImRUtil {
    my $cmd = shift;

    print ">>> " . $TI->CommandLine() . "\n";
    $TI->Arguments("-ORBInitRef ImplRepoService=file://$ti_imriorfile $cmd");
    return $TI->SpawnWaitKill($ti->ProcessStartWaitInterval());
}


$Srv = $imr->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                            "-orbuseimr 1 ".
                                            "$OBJ_REF_STYLE ".
                                            "-o $imr_m1iorfile ".
                                            "-i $imr_m2iorfile ".
                                            "-ORBInitRef ImplRepoService=file://$imr_imriorfile");
my $server_cmd = $Srv->Executable();
my $srv_server_cmd = $imr->LocalFile ($server_cmd);
$C1 = $c1->CreateProcess ("MessengerClient");

# Use url object reference style for readability, and startup timeout of 2 seconds.
# Unlike the chapter we'll forgo using -m, because we want to be able to run this
# as a test in our nightly builds, and multicast could interfere with other machines.
$ImR = $imr->CreateProcess ($implrepo_server, "-d $DEBUG_LEVEL ".
                                               "$OBJ_REF_STYLE ".
                                               "-t 2 ".
                                               "-o $imr_imriorfile");

$TI = $ti->CreateProcess ($tao_imr, "-ORBInitRef ImplRepoService=file://$ti_imriorfile ".
                                    "add MessengerService ".
                                    "-c \"$srv_server_cmd $OBJ_REF_STYLE -ORBUseIMR 1 -ORBInitRef ImplRepoService=file://$imr_imriorfile\"");

# We want the tao_imr executable to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell its Process object to ignore the setting of -ExeSubDir.
$TI->IgnoreExeSubDir(1);

CleanupOutput();

#### Start the example

if (SpawnWait($ImR, $implrepo_ior, $imr) != 0) {
    $ImR->Kill(); $ImR->TimedWait(1);
    exit 1;
}
if ($imr->GetFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($ti->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$ti_imriorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

if (SpawnWait($Srv, $messenger2_ior, $imr) != 0) {
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($messenger1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_m1iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($messenger1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_m1iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($imr->GetFile ($messenger2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$imr_m2iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($messenger2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_m2iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}

$C1->Arguments($c1_m1iorfile);
if (SpawnWaitKill($C1, $c1) != 0) {
    print STDERR "Error : Client 1 failed to run correctly.";
    $status = 1;
}

$C1->Arguments($c1_m2iorfile);
if (SpawnWaitKill($C1, $c1) != 0) {
    print STDERR "Error : Client 2 failed to run correctly.";
    $status = 1;
}

#Since we didn't start the ImR with a known endpoint, we'll
#have to use the tao_imr utility to create a our urls.
RunImRUtil("ior MessengerService/Object1 -f $ti_m3iorfile");
RunImRUtil("ior MessengerService/Object2 -f $ti_m4iorfile");
if ($ti->GetFile ($messenger3_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ti_m3iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($messenger3_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_m3iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($ti->GetFile ($messenger4_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ti_m4iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}
if ($c1->PutFile ($messenger4_ior) == -1) {
    print STDERR "ERROR: cannot set file <$c1_m4iorfile>\n";
    $ImR->Kill (); $ImR->TimedWait (1);
    exit 1;
}


$C1->Arguments($c1_m3iorfile);
if (SpawnWaitKill($C1, $c1) != 0) {
    print STDERR "Error : Client 3 failed to run correctly.";
    $status = 1;
}

# Notice that this one results in two calls through the ImR, because
# we bound an indirect reference in the IORTable. If we were using
# PER_CLIENT activation, this would be bad.
$C1->Arguments($c1_m4iorfile);
if (SpawnWaitKill($C1, $c1) != 0) {
    print STDERR "Error : Client 4 failed to run correctly.";
    $status = 1;
}

$Srv_status = $Srv->TerminateWaitKill ($imr->ProcessStopWaitInterval());

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
