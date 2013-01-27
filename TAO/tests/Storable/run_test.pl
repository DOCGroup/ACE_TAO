eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Copy;

my $status = 0;

my $test1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $persistent_file = "test.dat";
$test1->DeleteFile ($persistent_file);

my $num_loops = 10;

$T1 = $test1->CreateProcess ("test", "-i 0 -n $num_loops");

my $test2 = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

$T2 = $test2->CreateProcess ("test", "-i 1 -n $num_loops");

$test1_status = $T1->Spawn ();

if ($test1_status != 0) {
    print STDERR "ERROR: test 1 returned $test1_status\n";
    $status = 1;
}

$test2_status = $T2->SpawnWaitKill ($test2->ProcessStartWaitInterval());

if ($test2_status != 0) {
    print STDERR "ERROR: test 2 returned $test2_status\n";
    $status = 1;
}

$test1_status = $T1->WaitKill ($test1->ProcessStopWaitInterval());

if ($test1_status != 0) {
    print STDERR "ERROR: test 1 returned $test1_status\n";
    $status = 1;
}

$test1->DeleteFile ($persistent_file);

sub test_backup_recovery($)
{
    my $bad_file = shift;

    my $status = 0;

    print STDERR "Testing recovery of backup file for $bad_file.\n";

    my $backup_file = "test.dat.bak";
    copy($bad_file, "test.dat") or die "Copy failed: $!";
    copy("data_files/good.dat", $backup_file) or die "Copy failed: $!";

    $T = $test1->CreateProcess ("test", "-b");
    my $test_status = $T->SpawnWaitKill ($test1->ProcessStartWaitInterval());
    if ($test_status != 0) {
	$status = 1;
	print STDERR "ERROR: Backup recovery test using $bad_file failed\n";
    }
    else {
	print STDERR "Recovery successful\n";
    }

    if (-e $backup_file) {
	$test1->DeleteFile ($backup_file);
    }
    else {
	$status = 1;
	print STDERR "ERROR: Backup file was not created\n";
    }
    $test1->DeleteFile ($persistent_file);

    return $status;
}

@bad_files = (
    "data_files/bad_binary.dat",
    "data_files/bad_int.dat",
    "data_files/bad_string.dat",
    "data_files/bad_unsigned_int.dat"
    );

foreach $bad_file (@bad_files) {
    if (test_backup_recovery ($bad_file) != 0) {
	$status = 1;
    }
}

exit $status
