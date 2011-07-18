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
$idl_file = PerlACE::LocalFile ("test.idl");

# The IDL compiler
$TAO_IDL = new PerlACE::Process ("$tao_idl");
if (exists $ENV{HOST_ROOT}) {
  $TAO_IDL->IgnoreHostRoot(1);
}
$ENV{'INCLUDE'} = ":$ENV{TAO_ROOT}/orbsvcs/:$ENV{TAO_ROOT}/:";

$TAO_IDL->Arguments ("-Se -hs _skel.h -hc _stub.h -I$ENV{TAO_ROOT} -I$ENV{TAO_ROOT}/orbsvcs/ test.idl");
$TAO_IDL->SpawnWaitKill (60);

$found = 0;

$stub_h = PerlACE::LocalFile("test_stub.h");
open (STUB_HANDLE, "<$stub_h");
while ($line = <STUB_HANDLE>)
{
    # Process the line.
    chomp $line;

    if ($line =~ /^\#include \"orbsvcs\/FT_CORBA_ORBC\.h\"$/) {
        print "INFO: orbsvcs/FT_CORBA_ORBC.h\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/CosNotification_stub\.h\"$/) {
        print "INFO: orbsvcs/CosNotification_stub.h\n";
        $found++;
    }
    if ($line =~ /\#include \"orbsvcs\/PortableGroupC\.h\"$/) {
        print "INFO: orbsvcs/PortableGroupC.h\n";
        $found++;
    }
}
close(STUB_HANDLE);

$skel_h = PerlACE::LocalFile("test_skel.h");
open (SKEL_HANDLE, "<$skel_h");
while ($line = <SKEL_HANDLE>)
{
    # Process the line.
    chomp $line;

    if ($line =~ /^\#include \"test_stub\.h\"$/) {
        print "INFO: test_stub.h\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/FT_CORBA_ORBS\.h\"$/) {
        print "INFO: orbsvcs/FT_CORBA_ORBC.h\n";
        $found++;
    }
    if ($line =~ /^\#include \"orbsvcs\/CosNotification_skel\.h\"$/) {
        print "INFO: orbsvcs/CosNotification_skel.h\n";
        $found++;
    }
    if ($line =~ /\#include \"orbsvcs\/PortableGroupS\.h\"$/) {
        print "INFO: orbsvcs/PortableGroupS.h\n";
        $found++;
    }
}
close(SKEL_HANDLE);

unlink <*.cpp *.inl *.h>;

if ($found == 7) {
    print "INFO: Test passed!\n";
    exit 0;
} else {
    print STDERR "ERROR: Custom endings are incorrectly applied.\n";
    exit 1;
}
