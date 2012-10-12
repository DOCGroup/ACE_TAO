eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

# The location of the tao_idl utility - depends on O/S
my $tao_ifr = "undefined";
if ($^O eq "MSWin32"){
   $tao_ifr = "$ENV{ACE_ROOT}/bin/tao_ifr";
}
else{
   $tao_ifr = "../../../IFR_Service/tao_ifr";
}

my %isa = ();
$isa{"InterfaceRoot::get_root"}      = "operation";
$isa{"InterfaceRoot::get_all_roots"} = "operation";
$isa{"InterfaceRoot::root_attr1"}    = "attribute";
$isa{"InterfaceRoot::root_attr2"}    = "attribute";
$isa{"InterfaceRoot::root_attr3"}    = "attribute";
$isa{"SubInterface::get_root"}       = "operation";
$isa{"SubInterface::get_all_roots"}  = "operation";
$isa{"SubInterface::set_sub"}        = "operation";
$isa{"SubInterface::get_all_subs"}   = "operation";
$isa{"SubInterface::root_attr1"}     = "attribute";
$isa{"SubInterface::root_attr2"}     = "attribute";
$isa{"SubInterface::root_attr3"}     = "attribute";
$isa{"SubInterface::sub_attr1"}      = "attribute";
$isa{"SubInterface::sub_attr2"}      = "attribute";

my %got = ();
$got{"InterfaceRoot::get_root"}      = 13;
$got{"InterfaceRoot::get_all_roots"} = 13;
$got{"InterfaceRoot::root_attr1"}    = 13;
$got{"InterfaceRoot::root_attr2"}    = 13;
$got{"InterfaceRoot::root_attr3"}    = 13;
$got{"SubInterface::get_root"}       = 13;
$got{"SubInterface::get_all_roots"}  = 13;
$got{"SubInterface::set_sub"}        = 13;
$got{"SubInterface::get_all_subs"}   = 13;
$got{"SubInterface::root_attr1"}     = 13;
$got{"SubInterface::root_attr2"}     = 13;
$got{"SubInterface::root_attr3"}     = 13;
$got{"SubInterface::sub_attr1"}      = 13;
$got{"SubInterface::sub_attr2"}      = 13;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $iorbase = "if_repo.ior";
my $testoutputfilename = "IFR_Inheritance_test.output";

my $log1 = "server1.log";
my $log2 = "server2.log";
my $log3 = "client.log";

my $log1_test = $test->LocalFile ($log1);
my $log2_test = $test->LocalFile ($log2);
my $log3_test = $test->LocalFile ($log3);

$test->DeleteFile ($log1);
$test->DeleteFile ($log2);
$test->DeleteFile ($log3);

my $test_iorfile = $test->LocalFile ($iorbase);
$test->DeleteFile($iorbase);

$SV1 = $test->CreateProcess ("../../../IFR_Service/tao_ifr_service",
                              " -o $test_iorfile " .
                              "-ORBLogFile $log1_test");

$SV2 = $test->CreateProcess ("$tao_ifr",
                              " IFR_Inheritance_Test.idl " .
                              "-ORBInitRef InterfaceRepository=file://$test_iorfile " .
                              "-ORBLogFile $log2_test");

$CL = $test->CreateProcess ("IFR_Inheritance_Test",
                              "-ORBInitRef InterfaceRepository=file://$test_iorfile " .
                              "-ORBLogFile $log3_test");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($test->WaitForFileTimed ($iorbase,
                               $test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$test_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->SpawnWaitKill ($test->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

my $type = "";
my $name = "";

foreach $result_file ("$log3_test"){
    open(THELOG, "< $result_file") or die "could not open the saved log";
    while (<THELOG>) {
        if (/^attribute/ || "^operation") {
            chomp($_);
            ($type, $name) = split(/ /, $_);
            #print "found $type $name\n";
            if ($isa{$name} eq $type) {
                #print "got $name\n";
                $got{$name} = 1;
            }
            else {
                print STDERR "ERROR: $type $name was not expected\n";
                $status = 1;
            }
        }
        else {
            print STDOUT "ERROR: unexpected line: $_\n";
        }
    }
    close(THELOG);
}

### did we get all of the expected attributes and operations?
my $key = "";
foreach $key (keys (%got)) {
    #print "got\{$key\} = $got{$key}\n";
    if (! exists $isa{$key} ) {
        print STDERR "CONFIG ERROR: \$got has $key but \$isa does not\n";
        $status = 1;
    }
    if ($got{$key} == 13) {
        print STDERR "ERROR: $isa{$key} $key was not found\n";
        $status = 1;
    }
}

$server_status = $SV1->TerminateWaitKill ($test->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$test->DeleteFile ($log1);
$test->DeleteFile ($log2);
$test->DeleteFile ($log3);
$test->DeleteFile($iorbase);

exit $status;
