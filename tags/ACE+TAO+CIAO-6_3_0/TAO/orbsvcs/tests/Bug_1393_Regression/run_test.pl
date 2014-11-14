eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my @generated_files = ("testC.h", "testC.cpp", "testC.inl", "testS.h", "testS.cpp");

# The location of the IFR_Service binary
my $ifr_service_bin = "../../IFR_Service";

# The location of the tao_ifr utility binary
my $tao_ifr_bin = "$ENV{ACE_ROOT}/bin";

# The location of the tao_idl utility binary
my $tao_idl_bin = "$ENV{ACE_ROOT}/bin";

my $service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client  = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $taoifr  = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

# The idl file to be used for the test
my $test_idl = "test.idl";
my $ifr_ior  = "ifr.ior";
my $service_test_idl = $service->LocalFile ($test_idl);
my $taoifr_test_idl = $taoifr->LocalFile ($test_idl);
my $service_ifr_ior = $service->LocalFile($ifr_ior);
my $taoifr_ifr_ior = $taoifr->LocalFile($ifr_ior);
my $client_ifr_ior = $client->LocalFile($ifr_ior);
$service->DeleteFile ($ifr_ior);
$client->DeleteFile ($ifr_ior);
$taoifr->DeleteFile ($ifr_ior);

$TAO_IDL     = $service->CreateProcess("$tao_idl_bin/tao_idl", "$service_test_idl");
$IFR_SERVICE = $service->CreateProcess("$ifr_service_bin/tao_ifr_service", "-o $service_ifr_ior");
$TAO_IFR     = $service->CreateProcess("$tao_ifr_bin/tao_ifr",
                                       "-ORBInitRef InterfaceRepository=file://$taoifr_ifr_ior ".
                                       "$taoifr_test_idl");
$CL = $client->CreateProcess("client", "-ORBInitRef InterfaceRepository=file://$client_ifr_ior");

# Create an idl file contaning an interface with a C++ reserved word name
open(HANDLE, ">$service_test_idl");
print HANDLE <<_EOF_
interface int {
};
_EOF_
;
close(HANDLE);

my $result = $TAO_IDL->SpawnWaitKill ($service->ProcessStartWaitInterval() + 15);

# Search one of the stub files for the munged for of the interface name
if ($result == 0) {
    $result = 1;
    open (HANDLE, "<testC.h");
    while (<HANDLE>) {
        if (/^class\s+_cxx_int/) {
            $result = 0;
            last;
        }
    }
    close (HANDLE);
}

# Check result of idl compiler test
if ($result != 0) {
    print STDERR "ERROR: Bug #1393 Idl compiler test failed\n";
    $status = 1;
}

# Delete files created during IDL compilation
foreach $file(@generated_files) {
    $service->DeleteFile ($file)
}

# Start the IFR service
$result = $IFR_SERVICE->Spawn();

if ($result != 0) {
    print STDERR "ERROR: IFR service returned $result\n";
    exit 1;
}

if ($service->WaitForFileTimed ($ifr_ior, $service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find $service_ifr_ior\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    $status = 1;
}

if ($service->GetFile ($test_idl) == -1) {
    print STDERR "ERROR: cannot retrieve file <$service_test_idl>\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    exit 1;
}

if ($service->GetFile ($ifr_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$service_ifr_ior>\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    exit 1;
}

if ($taoifr->PutFile ($test_idl) == -1) {
    print STDERR "ERROR: cannot set file <$taoifr_test_idl>\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    exit 1;
}

if ($taoifr->PutFile ($ifr_ior) == -1) {
    print STDERR "ERROR: cannot set file <$taoifr_ifr_ior>\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ifr_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_ifr_ior>\n";
    $IFR_SERVICE->Kill (); $IFR_SERVICE->TimedWait (1);
    exit 1;
}

# Use the iao_ifr utility to add our test IDL
$result = $TAO_IFR->SpawnWaitKill ($taoifr->ProcessStartWaitInterval() + 15);

if ($result != 0) {
   print STDERR "ERROR: Failed to add IDL to interface repository\n";
   $status = 1;
}

# Invoke the client code that retrieves the interface and checks it's not munged
$result = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 15);

if ($result != 0) {
   print STDERR "ERROR: Interface definition incorrect in IFR\n";
   $status = 1;
}

# Tidy up
$IFR_SERVICE->TerminateWaitKill ($service->ProcessStopWaitInterval());

$service->DeleteFile ($test_idl);
$taoifr->DeleteFile ($test_idl);
$service->DeleteFile ($ifr_ior);
$client->DeleteFile ($ifr_ior);
$taoifr->DeleteFile ($ifr_ior);

exit $status;
