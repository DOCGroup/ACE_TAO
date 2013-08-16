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

my $stdout_file        = "test.out";
my $stderr_file        = "test.err";
my $test_stdout_file = $test1->LocalFile ($stdout_file);
my $test_stderr_file = $test1->LocalFile ($stderr_file);

sub cat_file($)
{
    my $file_name = shift;
    if (-s $file_name ) # size of file is greater than zero
    {
        open TESTFILE, $file_name or die "Couldn't open file: $!";
        my @teststring = <TESTFILE>; # read in all of the file
        print STDERR "\n@teststring\n";
        close TESTFILE;
    }
}

sub redirect_output()
{
    open (OLDOUT, ">&", \*STDOUT) or die "Can't dup STDOUT: $!";
    open (OLDERR, ">&", \*STDERR) or die "Can't dup STDERR: $!";
    open STDERR, '>', $test_stderr_file;
    open STDOUT, '>', $test_stdout_file;
}

sub restore_output()
{
    open (STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!";
    open (STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!";
}


my $persistent_file = "test.dat";
$test1->DeleteFile ($persistent_file);

my $num_loops = 5;
my $loop_sleep_msec = 200;

if ($#ARGV >= 0) {
    for (my $i = 0; $i <= $#ARGV; $i++) {
	if ($ARGV[$i] eq '-num_loops') {
	    $i++;
	    $num_loops = $ARGV[$i];
	}
	elsif ($ARGV[$i] eq "-loop_sleep") {
	    $i++;
	    $loop_sleep_msec = $ARGV[$i];
	}
	else {
	    print "Usage: run_test.pl ".
		"[-num_loops <num=$num_loops>] ".
		"[-loop_sleep <msec=$loop_sleep_msec>]\n";
	    exit 1;
	}
    }
}

$T1 = $test1->CreateProcess ("test", "-i 0 -n $num_loops");

my $test2 = PerlACE::TestTarget::create_target (2) || die "Create target 1 failed\n";

$T2 = $test2->CreateProcess ("test", "-i 1 -n $num_loops -s $loop_sleep_msec");

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
    # Redirect output so that expected error messages are not interpreted as
    # test failure and rely instead of return status.
    redirect_output();
    my $test_status = $T->SpawnWaitKill ($test1->ProcessStartWaitInterval());
    restore_output();
    if ($test_status != 0) {
	$status = 1;
	print STDERR "ERROR: Backup recovery test using $bad_file failed\n";
	cat_file($test_stderr_file);
	cat_file($test_stdout_file);
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
    $test1->DeleteFile ($stdout_file);
    $test1->DeleteFile ($stderr_file);
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
