eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::stat;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sv1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $re1 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $re2 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $di = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

# amount of delay between running the servers

$sleeptime = 10;

$nsiorfile = "ns.ior";
$outputfile1 = "output1";
$outputfile2 = "output2";
$inputfile = "test_input";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv_nsiorfile = $sv->LocalFile ($nsiorfile);
my $sv1_nsiorfile = $sv1->LocalFile ($nsiorfile);
my $re1_nsiorfile = $re1->LocalFile ($nsiorfile);
my $re2_nsiorfile = $re2->LocalFile ($nsiorfile);
my $di_nsiorfile = $di->LocalFile ($nsiorfile);
my $re1_outputfile1 = $re1->LocalFile ($outputfile1);
my $re2_outputfile2 = $re2->LocalFile ($outputfile2);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di->DeleteFile ($nfsiorfile);
$sv->DeleteFile ($inputfile);
$sv1->DeleteFile ($inputfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

# generate test stream data
$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $sv_inputfile = $sv->LocalFile ($inputfile);
my $sv1_inputfile = $sv1->LocalFile ($inputfile);
if ($sv->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_inputfile>\n";
    exit 1;
}
if ($sv1->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_inputfile>\n";
    exit 1;
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");
$SV = $sv->CreateProcess ("sender",
                          " -ORBInitRef NameService=file://$sv_nsiorfile ".
                          "-s sender -r 10 ".
                          "-f $sv_inputfile");
$SV1 = $sv1->CreateProcess ("sender",
                            " -ORBInitRef NameService=file://$sv1_nsiorfile ".
                            "-s sender -r 10 ".
                            "-f $sv1_inputfile");
$RE1 = $re1->CreateProcess ("receiver",
                            " -ORBInitRef NameService=file://$re1_nsiorfile ".
                            "-s distributer -r receiver1 ".
                            "-f $re1_outputfile1");
$RE2 = $re2->CreateProcess ("receiver",
                            " -ORBInitRef NameService=file://$re2_nsiorfile ".
                            "-s distributer -r receiver2 ".
                            "-f $re2_outputfile2");
$DI = $di->CreateProcess ("distributer",
                          " -ORBInitRef NameService=file://$di_nsiorfile ".
                          "-s sender -r distributer ");

print STDERR "\nReceiver 1 --> Receiver 2 --> Distributer --> Sender\n\n";

print STDERR "Starting Naming Service\n";

$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
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
if ($sv1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Receiver 1\n";
$RE1_status = $RE1->Spawn ();
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Receiver 2\n";
$RE2_status = $RE2->Spawn ();

if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Distributer\n";
$DI_status = $DI->Spawn ();
if ($DI_status != 0) {
    print STDERR "ERROR: Distributer returned $DI_status\n";
    $DI->Kill (); $DI->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Sender\n";
$SV_status = $SV->SpawnWaitKill ($sv->ProcessStartWaitInterval()+985);
if ($SV_status != 0) {
    print STDERR "ERROR: Server returned $SV_status\n";
    $status = 1;
}

$DI_status = $DI->TerminateWaitKill ($di->ProcessStopWaitInterval()+985);
if ($DI_status != 0) {
    print STDERR "ERROR: distributer returned $DI_status\n";
    $status = 1;
}

$RE1_status = $RE1->TerminateWaitKill ($re1->ProcessStopWaitInterval()+985);
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $status = 1;
}

$RE2_status = $RE2->TerminateWaitKill ($re2->ProcessStopWaitInterval()+985);
if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval()+985);
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di->DeleteFile ($nfsiorfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

print STDERR "\nDistributer --> Receiver 1 --> Receiver 2 --> Sender\n\n";

print STDERR "Starting Naming Service\n";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
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
if ($sv1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Distributer\n";
$DI_status = $DI->Spawn ();
if ($DI_status != 0) {
    print STDERR "ERROR: Distributer returned $DI_status\n";
    $DI->Kill (); $DI->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Receiver 1\n";
$RE1_status = $RE1->Spawn ();
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $RE1->Kill (); $RE1->TimedWait (1);
    $DI->Kill (); $DI->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Receiver 2\n";
$RE2_status = $RE2->Spawn ();
if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $DI->Kill (); $DI->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Sender\n";
$SV_status = $SV->SpawnWaitKill ($sv->ProcessStartWaitInterval()+985);
if ($SV_status != 0) {
    print STDERR "ERROR: Server returned $SV_status\n";
    $status = 1;
}

$DI_status = $DI->TerminateWaitKill ($di->ProcessStopWaitInterval()+985);
if ($DI_status != 0) {
    print STDERR "ERROR: distributer returned $DI_status\n";
    $status = 1;
}

$RE1_status = $RE1->TerminateWaitKill ($re1->ProcessStopWaitInterval()+985);
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $status = 1;
}

$RE2_status = $RE2->TerminateWaitKill ($re2->ProcessStopWaitInterval()+985);
if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval()+985);
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di->DeleteFile ($nfsiorfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

print STDERR "\nSender --> Receiver 1 --> Receiver 2 --> Distributer\n\n";

print STDERR "Starting Naming Service\n";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
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
if ($sv1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($re2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$re2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Sender\n";
$SV1_status = $SV1->Spawn ();
if ($SV1_status != 0) {
    print STDERR "ERROR: Sender returned $SV1_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Receiver 1\n";
$RE1_status = $RE1->Spawn ();
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $RE1->Kill (); $RE1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Receiver 2\n";

$RE2_status = $RE2->Spawn ();
if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Distributer\n";
$DI_status = $DI->Spawn ();
if ($DI_status != 0) {
    print STDERR "ERROR: Distributer returned $DI_status\n";
    $DI->Kill (); $DI->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SV1_status = $SV1->WaitKill ($sv1->ProcessStartWaitInterval()+985);
if ($SV1_status != 0) {
    print STDERR "ERROR: Sender 1 returned $SV1_status\n";
    $status = 1;
}

$DI_status = $DI->TerminateWaitKill ($di->ProcessStopWaitInterval()+985);
if ($DI_status != 0) {
    print STDERR "ERROR: distributer returned $DI_status\n";
    $status = 1;
}

$RE1_status = $RE1->TerminateWaitKill ($re1->ProcessStopWaitInterval()+985);
if ($RE1_status != 0) {
    print STDERR "ERROR: Receiver 1 returned $RE1_status\n";
    $status = 1;
}

$RE2_status = $RE2->TerminateWaitKill ($re2->ProcessStopWaitInterval()+985);
if ($RE2_status != 0) {
    print STDERR "ERROR: Receiver 2 returned $RE2_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval()+985);
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di->DeleteFile ($nfsiorfile);
$sv->DeleteFile ($inputfile);
$sv1->DeleteFile ($inputfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

exit $status;
