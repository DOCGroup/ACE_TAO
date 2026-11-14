eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Compare;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cl = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

# amount of delay between running the servers

$sleeptime = 2;

$nsiorfile = "ns.ior";
$outputfile = "output";
$inputfile = "test_input";
$server_outputfile = "server_output";
$client_outputfile = "client_output";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv_nsiorfile = $sv->LocalFile ($nsiorfile);
my $cl_nsiorfile = $cl->LocalFile ($nsiorfile);
my $sv_outputfile = $sv->LocalFile ($outputfile);
my $cl_outputfile = $cl->LocalFile ($outputfile);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cl->DeleteFile ($inputfile);
$sv->DeleteFile ($outputfile);
$cl->DeleteFile ($outputfile);

# generate test stream data
$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $cl_inputfile = $cl->LocalFile ($inputfile);
if ($cl->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_inputfile>\n";
    exit 1;
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");
$SV = $sv->CreateProcess ("receiver",
                          " -ORBDebugLevel $debug_level ".
                          " -ORBInitRef NameService=file://$sv_nsiorfile ".
                          "-f $sv_outputfile");
$CL = $cl->CreateProcess ("sender",
                          " -ORBInitRef NameService=file://$cl_nsiorfile ".
                          "-f $cl_inputfile -r 2");

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
if ($cl->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Receiver\n";
$SV_status = $SV->Spawn ();
if ($SV_status != 0) {
    print STDERR "ERROR: Receiver returned $SV_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep $sleeptime;

print STDERR "Starting Sender\n";
$CL_status = $CL->SpawnWaitKill ($cl->ProcessStartWaitInterval()+185);
if ($CL_status != 0) {
    print STDERR "ERROR: sender returned $CL_status\n";
    $status = 1;
}

$SV_status = $SV->TerminateWaitKill ($sv->ProcessStopWaitInterval());
if ($SV_status != 0) {
    print STDERR "ERROR: receiver returned $SV_status\n";
    $status = 1;
}

if ($sv->GetFile ($outputfile, $server_outputfile) == -1) {
    print STDERR "ERROR: cannot retrieve output file <$sv_outputfile>\n";
    $status = 1;
}
elsif (File::Compare::compare ($inputfile, $server_outputfile) != 0) {
    print STDERR "ERROR: receiver output differs from input file\n";
    $status = 1;
}

if ($cl->GetFile ($outputfile, $client_outputfile) == -1) {
    print STDERR "ERROR: cannot retrieve output file <$cl_outputfile>\n";
    $status = 1;
}
elsif (File::Compare::compare ($inputfile, $client_outputfile) != 0) {
    print STDERR "ERROR: sender output differs from input file\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cl->DeleteFile ($inputfile);
$sv->DeleteFile ($outputfile);
$cl->DeleteFile ($outputfile);
unlink ($server_outputfile);
unlink ($client_outputfile);

exit $status;
