eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

my $status =0;
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$SV = $server->CreateProcess ("main");
$test = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());
if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status= 1;
}

#Redirect the screen output to the null device.
open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . File::Spec->devnull());
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

# The location of the tao_idl utility - depends on O/S
if ($^O eq "MSWin32"){
   $tao_idl = "../../bin/tao_idl";
}
else{
   $tao_idl = "../TAO_IDL/tao_idl";
}

my $server2 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $input_file1 = $server2->LocalFile ("invalid_scoping.idl");

# Compile the IDL
$SV = $server2->CreateProcess ("$tao_idl", "$input_file1");
$test = $SV->SpawnWaitKill ($server2->ProcessStartWaitInterval());

#Redirect the null device output back to the screen
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");
if ($test== 0) {
    print STDERR "ERROR: tao_idl returned $test for $input_file1, should have failed!\n";
    $status = 1;
}

exit $status;
