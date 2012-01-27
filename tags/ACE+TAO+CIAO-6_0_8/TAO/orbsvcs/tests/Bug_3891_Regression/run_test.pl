# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $status = 0;
my $debug_level = '0';

foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $imr = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $si = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $implrepo_server = "$ENV{TAO_ROOT}/orbsvcs/ImplRepo_Service/tao_imr_locator";

my $implrepo_ior = "implrepo.ior";

my $imr_imriorfile = $imr->LocalFile ($implrepo_ior);
my $si_imriorfile = $si->LocalFile ($implrepo_ior);

# Make sure the files are gone, so we can wait on them.
$imr->DeleteFile ($implrepo_ior);
$si->DeleteFile ($implrepo_ior);

my $SVR = $imr->CreateProcess ("server", "-ORBDebugLevel $debug_level " .
                               "-ORBUseImr 1 " .
                               "-ORBInitRef ImplRepoService=file://$si_imriorfile");

my $IR = $imr->CreateProcess ($implrepo_server, "-d 1 -t 5 " .
                              "-o $imr_imriorfile");
print ">>> " . $IR->CommandLine() . "\n";

my $IR_status = $IR->Spawn ();

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
if ($si->PutFile ($implrepo_ior) == -1) {
    print STDERR "ERROR: cannot set file <$si_imriorfile>\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}

print ">>> " . $SVR->CommandLine() . "\n";

my $SVR_status = $SVR->SpawnWaitKill ($si->ProcessStartWaitInterval());

if ($SVR_status != 0) {
    print STDERR "ERROR: server returned $SVR_status\n";
    $IR->Kill (); $IR->TimedWait (1);
    exit 1;
}


$IR_status = $IR->TerminateWaitKill ($imr->ProcessStopWaitInterval());

if ($IR_status != 0) {
    print STDERR "ERROR: ImplRepo Server returned $IR_status\n";
    $status = 1;
}

$imr->DeleteFile ($implrepo_ior);
$si->DeleteFile ($implrepo_ior);

exit $status;
