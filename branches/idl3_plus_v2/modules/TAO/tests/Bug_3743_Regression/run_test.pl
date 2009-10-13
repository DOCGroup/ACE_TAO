eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$tao_idl = "$PerlACE::ACE_ROOT/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

# IDL file names
$idl_file = PerlACE::LocalFile ("Test.idl");

# The IDL compiler
$TAO_IDL = new PerlACE::Process ("$tao_idl");
if (exists $ENV{HOST_ROOT}) {
    $TAO_IDL->IgnoreHostRoot(1);
}

$TAO_IDL->Arguments ("-Se -DTAO -hs .skel.h -hc .stub.h -I$ENV{TAO_ROOT} -I$ENV{TAO_ROOT}/orbsvcs/ $idl_file");
$TAO_IDL->SpawnWaitKill (60);

$found = 0;

$stub_h = PerlACE::LocalFile("Test.stub.h");
open (STUB_HANDLE, "<$stub_h");
while ($line = <STUB_HANDLE>)
{
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

$skel_h = PerlACE::LocalFile("Test.skel.h");
open (SKEL_HANDLE, "<$skel_h");
while ($line = <SKEL_HANDLE>)
{
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

unlink <*.cpp *.inl *.h>;

if ($found == 5) {
    print "INFO: Test passed!\n";
    exit 0;
} else {
    print STDERR "ERROR: Custom endings are incorrectly applied.\n";
    exit 1;
}
