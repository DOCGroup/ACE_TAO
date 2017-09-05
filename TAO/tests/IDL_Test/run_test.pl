eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

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

my $tao_idl = "$ENV{ACE_ROOT}/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $input_file2 = $server2->LocalFile ("invalid_scoping1.idl");

# Compile the IDL
$SV = $server2->CreateProcess ("$tao_idl", "$input_file2");
$test2 = $SV->SpawnWaitKill ($server2->ProcessStartWaitInterval());

my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $input_file3 = $server3->LocalFile ("invalid_scoping2.idl");

# Compile the IDL
$SV = $server3->CreateProcess ("$tao_idl", "$input_file3");
$test3 = $SV->SpawnWaitKill ($server3->ProcessStartWaitInterval());

my $server4 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $input_file4 = $server4->LocalFile ("keyword_case_diff.idl");

# Compile the IDL
$SV = $server4->CreateProcess ("$tao_idl", "$input_file4");
$test4 = $SV->SpawnWaitKill ($server4->ProcessStartWaitInterval());

my $server5 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $input_file5 = $server5->LocalFile ("keyword_case_diff.idl");
my $result_filename5 = "keyword_case_diff.log";
my $result_file5 = $server5->LocalFile ($result_filename5);

open (STDERR, ">$result_file5") or die "can't redirect stderr: $!";

# Compile the IDL
$SV = $server5->CreateProcess ("$tao_idl", " -Cw $input_file5");
$test5 = $SV->SpawnWaitKill ($server5->ProcessStartWaitInterval());

open (STDERR, ">&STDOUT");

sub analyze_results {
    my $result_file = $_[0];

    if (! -r $result_file) {
        print STDERR "ERROR: cannot find $result_file\n";
        return 1;
    }

    my $match = 0;
    open (FILE, $result_file) or return -1;
    while (<FILE>) {
        $match = /Warning - spelling differs from IDL keyword only in case:/;
        last if $match;
    }
    close FILE;

    return $match ? 0 : -1;
}

$match5 = analyze_results($result_file5);
$server5->DeleteFile($result_filename5);

#Redirect the null device output back to the screen
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");
if ($test2== 0) {
    print STDERR "ERROR: tao_idl returned $test2 for $input_file2, should have failed!\n";
    $status = 1;
}
if ($test3== 0) {
    print STDERR "ERROR: tao_idl returned $test3 for $input_file3, should have failed!\n";
    $status = 1;
}
if ($test4== 0) {
    print STDERR "ERROR: tao_idl returned $test4 for $input_file4, should have failed!\n";
    $status = 1;
}
if ($test5!= 0) {
    print STDERR "ERROR: tao_idl -Cw returned $test5 for $input_file5, should have succeeded!\n";
    $status = 1;
}
if ($match5!= 0) {
    print STDERR "ERROR: tao_idl -Cw should have printed warning for $input_file5\n";
    $status = 1;
}

exit $status;
