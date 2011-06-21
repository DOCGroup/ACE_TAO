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
my $sv1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $sv2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $sv3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $re1 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $re2 = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $di1 = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $di2 = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";
my $di3 = PerlACE::TestTarget::create_target (9) || die "Create target 9 failed\n";
my $di4 = PerlACE::TestTarget::create_target (10) || die "Create target 10 failed\n";

# amount of delay between running the servers

$sleeptime = 10;
$distributor_time = 8;
$sender_time = 8;

$nsiorfile = "ns.ior";
$outputfile1 = "output1";
$outputfile2 = "output2";
$inputfile = "test_input";
$conffile = "components_svc".$PerlACE::svcconf_ext;

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv1_nsiorfile = $sv1->LocalFile ($nsiorfile);
my $sv2_nsiorfile = $sv2->LocalFile ($nsiorfile);
my $sv3_nsiorfile = $sv3->LocalFile ($nsiorfile);
my $re1_nsiorfile = $re1->LocalFile ($nsiorfile);
my $re2_nsiorfile = $re2->LocalFile ($nsiorfile);
my $di1_nsiorfile = $di1->LocalFile ($nsiorfile);
my $di2_nsiorfile = $di2->LocalFile ($nsiorfile);
my $di3_nsiorfile = $di3->LocalFile ($nsiorfile);
my $di4_nsiorfile = $di4->LocalFile ($nsiorfile);
my $sv1_conffile = $sv1->LocalFile ($conffile);
my $sv2_conffile = $sv2->LocalFile ($conffile);
my $sv3_conffile = $sv3->LocalFile ($conffile);
my $re1_conffile = $re1->LocalFile ($conffile);
my $re2_conffile = $re2->LocalFile ($conffile);
my $di1_conffile = $di1->LocalFile ($conffile);
my $di2_conffile = $di2->LocalFile ($conffile);
my $di3_conffile = $di3->LocalFile ($conffile);
my $di4_conffile = $di4->LocalFile ($conffile);
my $re1_outputfile1 = $re1->LocalFile ($outputfile1);
my $re2_outputfile2 = $re2->LocalFile ($outputfile2);
$ns->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$sv2->DeleteFile ($nsiorfile);
$sv3->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di1->DeleteFile ($nfsiorfile);
$di2->DeleteFile ($nfsiorfile);
$di3->DeleteFile ($nfsiorfile);
$di4->DeleteFile ($nfsiorfile);
$sv1->DeleteFile ($inputfile);
$sv2->DeleteFile ($inputfile);
$sv3->DeleteFile ($inputfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

# generate test stream data
$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $sv1_inputfile = $sv1->LocalFile ($inputfile);
my $sv2_inputfile = $sv2->LocalFile ($inputfile);
my $sv3_inputfile = $sv3->LocalFile ($inputfile);
if ($sv1->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_inputfile>\n";
    exit 1;
}
if ($sv2->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv2_inputfile>\n";
    exit 1;
}
if ($sv3->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv3_inputfile>\n";
    exit 1;
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -o $ns_nsiorfile");
$SV1 = $sv1->CreateProcess ("sender",
                          " -ORBDebugLevel $debug_level ".
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $sv1_conffile ".
                          "-ORBInitRef NameService=file://$sv1_nsiorfile ".
                          "-s sender -r 30 ".
                          "-f $sv1_inputfile");
$SV2 = $sv2->CreateProcess ("sender",
                          " -ORBDebugLevel $debug_level ".
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $sv2_conffile ".
                          "-ORBInitRef NameService=file://$sv2_nsiorfile ".
                          "-s sender -r 30 ".
                          "-f $sv2_inputfile");
$SV3 = $sv3->CreateProcess ("sender",
                          " -ORBDebugLevel $debug_level ".
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $sv3_conffile ".
                          "-ORBInitRef NameService=file://$sv3_nsiorfile ".
                          "-s sender -r 30 ".
                          "-f $sv3_inputfile");
$RE1 = $re1->CreateProcess ("receiver",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $re1_conffile ".
                          "-ORBInitRef NameService=file://$re1_nsiorfile ".
                          "-s distributer -r receiver1 ".
                          "-f $re1_outputfile1");
$RE2 = $re2->CreateProcess ("receiver",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $re2_conffile ".
                          "-ORBInitRef NameService=file://$re2_nsiorfile ".
                          "-s distributer -r receiver2 ".
                          "-f $re2_outputfile2");
$DI1 = $di1->CreateProcess ("distributer",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $di1_conffile ".
                          "-ORBInitRef NameService=file://$di1_nsiorfile ".
                          "-s sender -r distributer ");
$DI2 = $di2->CreateProcess ("distributer",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $di2_conffile ".
                          "-ORBInitRef NameService=file://$di2_nsiorfile ".
                          "-s sender -r distributer ");
$DI3 = $di3->CreateProcess ("distributer",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $di3_conffile ".
                          "-ORBInitRef NameService=file://$di3_nsiorfile ".
                          "-s sender -r distributer ");
$DI4 = $di4->CreateProcess ("distributer",
                          "-ORBDottedDecimalAddresses 1 ".
                          " -ORBSvcConf $di4_conffile ".
                          "-ORBInitRef NameService=file://$di4_nsiorfile ".
                          "-s sender -r distributer ");

print STDERR "\nReceiver 1 --> Receiver 2 --> Distributer 1 --> Sender1 --> Distributer 2 --> Distributer 3 --> Sender2 --> Sender3 --> Distributer4\n\n";

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
if ($sv1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sv2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sv3->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv3_nsiorfile>\n";
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
if ($di1->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di1_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di2->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di2_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di3->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di3_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($di4->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$di4_nsiorfile>\n";
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

print STDERR "Starting Distributer 1\n";
$DI1_status = $DI1->Spawn ();
if ($DI1_status != 0) {
    print STDERR "ERROR: Distributer 1 returned $DI1_status\n";
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Sender1\n";
$SV1_status = $SV1->Spawn ();
if ($SV1_status != 0) {
    print STDERR "ERROR: Sender 1 returned $SV1_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $distributor_time;

print STDERR "\nStarting Distributer 2\n\n";
$DI2_status = $DI2->Spawn ();
if ($DI2_status != 0) {
    print STDERR "ERROR: Distributer 2 returned $DI2_status\n";
    $DI2->Kill (); $DI2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $distributor_time;

print STDERR "\nStarting Distributer 3\n\n";
$DI3_status = $DI3->Spawn ();
if ($DI3_status != 0) {
    print STDERR "ERROR: Distributer 3 returned $DI3_status\n";
    $DI3->Kill (); $DI3->TimedWait (1);
    $DI2->Kill (); $DI2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sender_time;

print STDERR "Starting Sender2\n";
$SV2_status = $SV2->Spawn ();
if ($SV2_status != 0) {
    print STDERR "ERROR: Sender 2 returned $SV2_status\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $DI3->Kill (); $DI3->TimedWait (1);
    $DI2->Kill (); $DI2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sender_time;

print STDERR "Starting Sender3\n";
$SV3_status = $SV3->Spawn ();
if ($SV3_status != 0) {
    print STDERR "ERROR: Sender 3 returned $SV3_status\n";
    $SV3->Kill (); $SV3->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $DI3->Kill (); $DI3->TimedWait (1);
    $DI2->Kill (); $DI2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $distributor_time;

print STDERR "\nStarting Distributer 4\n\n";
$DI4_status = $DI4->Spawn ();
if ($DI4_status != 0) {
    print STDERR "ERROR: Distributer 4 returned $DI4_status\n";
    $DI4->Kill (); $DI4->TimedWait (1);
    $SV3->Kill (); $SV3->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $DI3->Kill (); $DI3->TimedWait (1);
    $DI2->Kill (); $DI2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $DI1->Kill (); $DI1->TimedWait (1);
    $RE2->Kill (); $RE2->TimedWait (1);
    $RE1->Kill (); $RE1->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SV2_status = $SV2->WaitKill ($sv2->ProcessStartWaitInterval()+285);
if ($SV2_status != 0) {
    print STDERR "ERROR: Sender 2 returned $SV2_status\n";
    $status = 1;
}

$SV3_status = $SV3->WaitKill ($sv3->ProcessStartWaitInterval()+285);
if ($SV3_status != 0) {
    print STDERR "ERROR: Sender 3 returned $SV3_status\n";
    $status = 1;
}

$SV1_status = $SV1->WaitKill ($sv1->ProcessStartWaitInterval()+285);
if ($SV1_status != 0) {
    print STDERR "ERROR: Sender 1 returned $SV1_status\n";
    $status = 1;
}

$DI3_status = $DI3->TerminateWaitKill ($di3->ProcessStopWaitInterval());
if ($DI3_status != 0) {
    print STDERR "ERROR: distributer 3 returned $DI3_status\n";
    $status = 1;
}

$DI2_status = $DI2->TerminateWaitKill ($di2->ProcessStopWaitInterval());
if ($DI2_status != 0) {
    print STDERR "ERROR: distributer 2 returned $DI2_status\n";
    $status = 1;
}

$DI1_status = $DI1->TerminateWaitKill ($di1->ProcessStopWaitInterval());
if ($DI1_status != 0) {
    print STDERR "ERROR: distributer 1 returned $DI1_status\n";
    $status = 1;
}

$RE1_status = $RE1->TerminateWaitKill ($re1->ProcessStopWaitInterval());
if ($RE1_status != 0) {
    print STDERR "ERROR: receiver 1 returned $RE1_status\n";
    $status = 1;
}

$RE2_status = $RE2->TerminateWaitKill ($re2->ProcessStopWaitInterval());
if ($RE2_status != 0) {
    print STDERR "ERROR: receiver 2 returned $RE2_status\n";
    $status = 1;
}

$DI4_status = $DI4->TerminateWaitKill ($di4->ProcessStopWaitInterval());
if ($DI4_status != 0) {
    print STDERR "ERROR: distributer 4 returned $DI4_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv1->DeleteFile ($nsiorfile);
$sv2->DeleteFile ($nsiorfile);
$sv3->DeleteFile ($nsiorfile);
$re1->DeleteFile ($nfsiorfile);
$re2->DeleteFile ($nfsiorfile);
$di1->DeleteFile ($nfsiorfile);
$di2->DeleteFile ($nfsiorfile);
$di3->DeleteFile ($nfsiorfile);
$di4->DeleteFile ($nfsiorfile);
$sv1->DeleteFile ($inputfile);
$sv2->DeleteFile ($inputfile);
$sv3->DeleteFile ($inputfile);
$re1->DeleteFile ($outputfile1);
$re2->DeleteFile ($outputfile2);

exit $status;
