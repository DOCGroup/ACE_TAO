

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

$status =0;

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . File::Spec->devnull());
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

my $tao_idl = "$ENV{ACE_ROOT}/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$input_file1 = $server->LocalFile ("local_inarg.idl");
$input_file2 = $server->LocalFile ("local_inoutarg.idl");
$input_file3 = $server->LocalFile ("local_outarg.idl");
$input_file4 = $server->LocalFile ("local_rettype.idl");

# Compile the IDL
$SV = $server->CreateProcess ("$tao_idl", "$input_file1");

$server_status1 = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

# Compile the IDL
$SV = $server->CreateProcess ("$tao_idl", "$input_file2");

$server_status2 = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

# Compile the IDL
$SV = $server->CreateProcess ("$tao_idl", "$input_file3");

$server_status3 = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

# Compile the IDL
$SV = $server->CreateProcess ("$tao_idl", "$input_file4");

$server_status4 = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

if ($server_status1 == 0) {
    print STDERR "ERROR: tao_idl returned $server_status1 for $input_file1, should have failed\n";
    $status = 1;
}

if ($server_status2 == 0) {
    print STDERR "ERROR: tao_idl returned $server_status2 for $input_file2, should have failed\n";
    $status = 1;
}

if ($server_status3 == 0) {
    print STDERR "ERROR: tao_idl returned $server_status3 for $input_file3, should have failed\n";
    $status = 1;
}

if ($server_status4 == 0) {
    print STDERR "ERROR: tao_idl returned $server_status4 for $input_file4, should have failed\n";
    $status = 1;
}

exit $status;
