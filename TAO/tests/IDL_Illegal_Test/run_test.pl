eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

my $status = 0;
my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $tao_idl = "$ENV{ACE_ROOT}/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">" . File::Spec->devnull());
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT");

foreach my $idl_file ("invalid_scoping1.idl", "invalid_scoping2.idl",
                      "keyword_case_diff.idl", "exception_misuse1.idl",
                      "exception_misuse2.idl") {
    my $input_file = $target->LocalFile ($idl_file);
    my $process = $target->CreateProcess ($tao_idl, $input_file);
    my $result = $process->SpawnWaitKill ($target->ProcessStartWaitInterval());
    if ($result == 0) {
        print STDERR "ERROR: tao_idl returned success for $input_file, should have failed\n";
        $status = 1;
    }
}

my $input_file = $target->LocalFile ("keyword_case_diff.idl");
my $result_filename = "keyword_case_diff.log";
my $result_file = $target->LocalFile ($result_filename);
open (STDERR, ">$result_file") or die "can't redirect stderr: $!";
my $process = $target->CreateProcess ($tao_idl, "-Cw $input_file");
my $result = $process->SpawnWaitKill ($target->ProcessStartWaitInterval());
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

if ($result != 0) {
    print STDERR "ERROR: tao_idl -Cw returned $result for $input_file, should have succeeded\n";
    $status = 1;
}

my $warning_found = 0;
if (open (FILE, $result_file)) {
    while (<FILE>) {
        $warning_found = /Warning - spelling differs from IDL keyword only in case:/;
        last if $warning_found;
    }
    close FILE;
}
else {
    print STDERR "ERROR: cannot read $result_file\n";
    $status = 1;
}

if (!$warning_found) {
    print STDERR "ERROR: tao_idl -Cw should have printed a warning for $input_file\n";
    $status = 1;
}

$target->DeleteFile ($result_filename);
exit $status;
