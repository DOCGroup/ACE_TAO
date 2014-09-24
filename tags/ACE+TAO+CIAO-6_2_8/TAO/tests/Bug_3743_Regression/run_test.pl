eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $tao_idl = "$PerlACE::ACE_ROOT/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

# IDL file names
my $test_idlfile = $test->LocalFile ("Test.idl");

# Files to delete after test
my @generated_files = ("TestC.cpp", "TestS.cpp", "TestC.inl", "Test.skel.h", "Test.stub.h");

# The IDL compiler
$TAO_IDL = $test->CreateProcess ("$tao_idl");
if (exists $ENV{HOST_ROOT}) {
    $TAO_IDL->IgnoreHostRoot(1);
}

$TAO_IDL->Arguments ("-Se -DTAO -hs .skel.h -hc .stub.h ".
                     "-I$ENV{TAO_ROOT} -I$ENV{TAO_ROOT}/orbsvcs/ $test_idlfile");

$test_status = $TAO_IDL->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);
if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    exit 1;
}

$found = 0;

$stub_h = $test->LocalFile("Test.stub.h");

open (STUB_HANDLE, "<$stub_h");

while ($line = <STUB_HANDLE>) {
    # Process the line.
    chomp $line;

    if ($line =~ /^\#include \"orbsvcs\/CosEventChannelAdminC\.h\"$/) {
        print "INFO: orbsvcs/CosEventChannelAdminC.h\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/CosTypedEventChannelAdminC\.h\"$/) {
        print "INFO: orbsvcs/CosTypedEventChannelAdminC.h\n";
        $found++;
    }
}

close(STUB_HANDLE);

$skel_h = $test->LocalFile("Test.skel.h");

open (SKEL_HANDLE, "<$skel_h");

while ($line = <SKEL_HANDLE>) {
    # Process the line.
    chomp $line;

    if ($line =~ /^\#include \"Test\.stub\.h\"$/) {
        print "INFO: Test.stub.hpp\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/CosEventChannelAdminS\.h\"$/) {
        print "INFO: orbsvcs/CosEventChannelAdminS.h\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/CosTypedEventChannelAdminS\.h\"$/) {
        print "INFO: orbsvcs/CosTypedEventChannelAdminS.h\n";
        $found++;
    }
}

close(SKEL_HANDLE);

foreach $generated_file (@generated_files) {
    $test->DeleteFile($generated_file);
}

if ($found == 5) {
    print "INFO: Test passed!\n";
} else {
    print STDERR "ERROR: Custom endings are incorrectly applied.\n";
    $status = 1;
}

exit $status;
